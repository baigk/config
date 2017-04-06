from util.Controller import Controller

class TestController1(Controller):
    
    def show(self, req, resp):
        resp('200 OK', [('Context-Type', 'text/plain')])
     
        raise Exception
       
        return 'TestController1::show->TestControl1'

    def __call__(self, env, resp):
        resp('200 OK', [('Context-Type', 'text/plain')])
#        raise Exception
        return 'TestController1'
                   
    
