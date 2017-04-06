from oslo_service import service
from oslo_config import cfg
import wsgi

CONF = cfg.CONF

class WSGIService(service.Service):
    def __init__(self, name, loader=None, max_url_len=None):
        self.name = name
        self.max_url_len = max_url_len
        self.loader = loader or wsgi.Loader(getattr(CONF, "paste_ini", None))
        self.app = self.loader.load_app(name)

        self.host = getattr(CONF, "%s_listen" % name, '0.0.0.0')
        self.port = getattr(CONF, "%s_listen_port" % name, 0)

        self.server = wsgi.server(name, self.app, host=self.host, port=self.port, max_url_len=max_url_len)

    def reset(self):
        self.server.reset()

    def start(self):
        self.server.start()

    def stop(self):
        self.server.stop()

    def wait(self):
        self.server.wait()
