import sys
import os.path
import inspect
import eventlet
import traceback
import socket
import eventlet.wsgi
import logging
from paste import deploy
from oslo_config import cfg
from util import Exception as exception
import webob.dec
import webob.exc
import routes
import routes.middleware

LOG = logging.getLogger(__name__)
class server(object):
    def __init__(self, name, application, host = '0.0.0.0', port = 0, max_url_len = None):
        #event.wsgi.MAX_HEADER_LINE = self.conf.max_header_line
        
        self.name   = name
        self.application = application
	self.host = host
	self.port = port
        self.max_url_len = max_url_len

    def start(self):
        try:
	     sock  = eventlet.listen((self.host, self.port), backlog=5, family=socket.AF_INET)
       
             sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
             sock.setsockopt(socket.SOL_SOCKET,socket.SO_KEEPALIVE, 1)
           
             self.pool = eventlet.GreenPool(size=100)
	     self.pool.spawn_n(self.run)
             self.sock = sock
             LOG.info("Start %s server", self.name)

        except:
            LOG.exception("start fail %s\n", (self.host, self.port))
    
    def wait(self):
        try:
            self.pool.waitall()
        except KeyboardInterrupt:
            pass
        except:
            LOG.exception("wait\n")

    def run(self):
        try:
            eventlet.wsgi.server(self.sock, self.application, custom_pool=self.pool,
                             url_length_limit=self.max_url_len,
                             log=LOG, debug=True)

        except:
            LOG.exception("run failed\n");
		

class Router(object):
    def __init__(self, mapper):
        self.mapper = mapper
        self._route = routes.middleware.RoutesMiddleware(self._dispatch, self.mapper)

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

    def _setup_routes(self):
        pass

class Resource(object):
	pass   

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

