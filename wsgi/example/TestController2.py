from util.Controller import SeriallizerController

class TestController2(SeriallizerController):
    def show(self, env, resp):
        print ";;;;;;;;;;;;;;;;;", self, env , resp
        #resp('200 OK', [('Context-Type', 'text/plain')])
        return 'TestController2::show hello world'

    def version(self, env, resp):
        resp('200 OK', [('Context-Type', 'text/plain')])
        return 'TestController2::version 1.0'

    def __call__(self, env, resp):
        resp('200 OK', [('Context-Type', 'text/plain')])
        return 'TestController2'


                    
    
