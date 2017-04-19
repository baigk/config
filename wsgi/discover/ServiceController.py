from wsgi.Controller import Controller
from discover.consul.std_agent import *

class ServiceController(Controller):
    def __init__(self):
        self.agent = std_agent.instance()

    def health(self, req):
        pass

    def update(self, req, **kwargs):
        ret = self.agent.update(eval(req.body))

    def get(self, req):
        return self.agent.get()
