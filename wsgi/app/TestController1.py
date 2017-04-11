from wsgi.Controller import Controller

class TestController1(Controller):
    
    def show_GET(self, req):
       #('200 OK', [('Context-Type', 'text/plain')])
     
        #raise Exception
       
        return 'TestController1::show_GET->TestControl1'

    def show_POST(self, req):
        #('200 OK', [('Context-Type', 'text/plain')])
     
        #raise Exception
       
        return 'TestController1::show_POST->TestControl1'

    def __call__(self, env, resp):
        resp('200 OK', [('Context-Type', 'text/plain')])

        return 'TestController1'
                   
    
