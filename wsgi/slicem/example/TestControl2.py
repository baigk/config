from util.Controller import SeriallizerController

class control(SeriallizerController):
    def show(self, env, resp):
        resp('200 OK', [('Context-Type', 'text/plain')])
        return 'TestContrl2::show hello world'

    def version(self, env, resp):
        resp('200 OK', [('Context-Type', 'text/plain')])
        return 'TestContrl2::version 1.0'

    def __call__(self, env, resp):
        resp('200 OK', [('Context-Type', 'text/plain')])
        return 'TestContrl2'


                    
    
