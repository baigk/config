import sys
import os.path
import inspect
import eventlet
import greenlet
import traceback
import socket
import eventlet.wsgi
import logging
from paste import deploy
from oslo_config import cfg
from oslo_utils import encodeutils
from oslo_service import service
import oslo_i18n as i18n
import Exception as exception
import webob.dec
import webob.exc
import routes
import routes.middleware
import Controller

LOG = logging.getLogger(__name__)
class server(service.ServiceBase):
    def __init__(self, name, application, host = '0.0.0.0', port = 0, max_url_len = None):
        #event.wsgi.MAX_HEADER_LINE = self.conf.max_header_line
        self.pool_size = 100 
        self.name   = name
        self.application = application
        self.host = host
        self.port = port
        self.max_url_len = max_url_len
        self.pool = eventlet.GreenPool(size=self.pool_size)
        self.server = None
        try:
             self.sock  = eventlet.listen((self.host, self.port), backlog=5, family=socket.AF_INET)
        except Exception:
             LOG.error(" %s listen fail %s:%s" % (name, self.host, self.port))
             raise

    def start(self):
        try:
            sock = self.sock.dup()    
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            sock.setsockopt(socket.SOL_SOCKET,socket.SO_KEEPALIVE, 1)
            LOG.info("Start %s server at %s:%s", self.name, self.host, self.port)

            wsgi_kwargs = {
                       'func': eventlet.wsgi.server,
                       'sock': sock,
                       'site': self.application,
                       'custom_pool': self.pool,
                       'log': LOG,
                       'debug': True,
                       }
            
            if self.max_url_len:
                wsgi_kwargs['url_length_limit'] = self.max_url_len

            self.server = eventlet.spawn(**wsgi_kwargs)

        except:
            LOG.exception("start fail %s\n", (self.host, self.port))
    
    def reset(self):
        self.pool.resize(self.pool_size)
 
    def wait(self):
        try:
            if self.server is not None:
                self.pool.waitall()
                self.server.wait()
        except greenlet.GreenletExit:
            LOG.info("WSGI server has stopped.")
    
    def stop(self):
        LOG.info("Stopping WSGI server.")

        if self.server is not None:
            self.pool.resize(0)
            self.server.kill()

class Router(object):
    def __init__(self, mapper):
        self.mapper = mapper
        self._route = routes.middleware.RoutesMiddleware(self._dispatch, self.mapper,  singleton=False)

    @webob.dec.wsgify
    def __call__(self, req):
        return self._route

    @staticmethod
    @webob.dec.wsgify
    def _dispatch(req):
        match = req.environ['wsgiorg.routing_args'][1]
        if not match:
            return webob.exc.HTTPNotFound()
        app = match['controller']

        return app

class APIRouter(Router):
    @classmethod
    def factory(cls, global_config, **local_config):
        return cls()

    def __init__(self):
        super(APIRouter, self).__init__(routes.Mapper())
        self.resources = {}
        self._setup_routes()
       
        self.resources['HEALTH'] = Controller.Controller.create_resource()
        self.mapper.connect(None, '/health', controller=self.resources['HEALTH'], action="health")

    def _setup_routes(self):
        pass

class Request(webob.Request):

    def best_match_content_type(self):
        supported = ('application/json',)
        bm = self.accept.best_match(supported)
        return bm or 'application/json'

    def get_content_type(self, allowed_content_types):
        if "Content-Type" not in self.headers:
            raise exception.InvalidContentType(content_type=None)

        content_type = self.content_type

        if content_type not in allowed_content_types:
            raise exception.InvalidContentType(content_type=content_type)
        else:
            return content_type

    def best_match_language(self):
        if not self.accept_language:
            return None
        all_languages = i18n.get_available_languages('sdme')
        return self.accept_language.best_match(all_languages)

class Resource(object):
    def __init__(self, controller, deserializer = None, serializer = None):
        self.controller = controller
        self.deserializer = deserializer
        self.serializer = serializer

    def translate_exception(self, exc, locale):

        err_msg = encodeutils.exception_to_unicode(exc)
        exc.message = i18n.translate(err_msg, locale)

        if isinstance(exc, webob.exc.HTTPError):
            exc.explanation = i18n.translate(exc.explanation, locale)
            exc.detail = i18n.translate(getattr(exc, 'detail', ''), locale)
        return exc
 
    @webob.dec.wsgify(RequestClass=Request)
    def __call__(self, request):
        action_args = self.get_action_args(request.environ)
        action = action_args.pop('action', None)
        content_type = request.params.get("ContentType")
        try:
            LOG.debug('Calling %(env)s', {'env' : request.environ})
            if self.deserializer:      
                deserialized_request = self.dispatch(self.deserializer, action, request)
                action_args.update(deserialized_request)

            action_result = self.dispatch(self.controller, action, request, **action_args)

        except TypeError as err:
            msg = 'The server could not comply with the request since it is either malformed or otherwise incorrect.'
            err = webob.exc.HTTPBadRequest(msg)
            http_exc = self.translate_exception(err, request.best_match_language())
            raise exception.HTTPExceptionDisguise(http_exc)
        except webob.exc.HTTPException as err:
            if not isinstance(err, webob.exc.HTTPError):
                raise

            if isinstance(err, webob.exc.HTTPServerError):
                LOG.error("Returning %(code)s to user: %(explanation)s", {'code': err.code, 'explanation': err.explanation})
            http_exc = self.translate_exception(err, request.best_match_language())
            raise exception.HTTPExceptionDisguise(http_exc)
        except Exception as err:
            LOG.exception(sys.exc_info())
            raise self.translate_exception(err, request.best_match_language())

        try:
            serializer = self.serializer
            if serializer is None:
                if content_type in ["JSON", "application/json", "text/plain"]:
                    serializer = serializers.JSONResponseSerializer()
                else:
                    serializer = serializers.XMLResponseSerializer()

            response = webob.Response(request=request)
            self.dispatch(serializer, action, response, action_result)
            return response

        except Exception:
            if content_type == "JSON":
                try:
                    err_body = action_result.get_unserialized_body()
                    serializer.default(action_result, err_body)
                except Exception:
                    LOG.warning("Unable to serialize exception response")

            return action_result 
        
    def dispatch(self, obj, action, *args, **kwargs):
        try:
            method = getattr(obj, action)
        except AttributeError:
            method = getattr(obj, 'default')
	
        try:
            return method(*args, **kwargs)
        except TypeError as exc:
            LOG.exception(exc)
            return webob.exc.HTTPBadRequest()
    
    def get_action_args(self, env):
        try:
            args = env['wsgiorg.routing_args'][1].copy()
        except Exception:
            return {}

        try:
            del args['controller']
        except KeyError:
            pass

        try:
            del args['format']
        except KeyError:
            pass

        return args         

class Loader(object):
    def __init__(self, config_path=None):
        self.config_path = None
        if not os.path.isabs(config_path):
            self.config_path = cfg.CONF.find_file(config_path)
        elif os.path.exists(config_path):
            self.config_path = config_path   
        
        if not self.config_path:
            raise exception.ConfigNotFound(path=config_path)

    def load_app(self, name):
        try:
            LOG.info("Loading app %(name)s from %(path)s",
                      {'name':name, 'path':self.config_path})

            return deploy.loadapp("config:%s" % self.config_path, name=name)
        except LookupError:
            LOG.exception("couldn't loolup app: %s" % name)
            raise exception.PasteAppNotFound("name %s, path %s" % (name, self.config_path))

