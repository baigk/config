from util.Controller import Controller

class control(Controller):
    def show(self, env, resp):
        resp('200 OK', [('Context-Type', 'text/plain')])
        return 'control::show->TestControl1'

    def __call__(self, env, resp):
        resp('200 OK', [('Context-Type', 'text/plain')])
        return 'TestControl1'
                   
    
