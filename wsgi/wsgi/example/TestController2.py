from wsgi.Controller import *

class TestController2(Controller):
    def show(self, req, **kwargs):
        resp  = kwargs.copy()
        resp.update({'TestController2' : 'show hello world'})
        resp.update({'env' : req.environ})
        resp.update({'body': req.body})

        return resp

    def version(self, res):
        return 'TestController2::version 1.0'

    def __call__(self, env, resp):
        resp('200 OK', [('Context-Type', 'text/plain')])
        return 'TestController2'


                    
    
