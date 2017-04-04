class control(object):
    def show(self, env, resp):
        resp('200 OK', [('Context-Type', 'text/plain')])
        return 'hello world'

    def __call__(self, env, resp):
        resp('200 OK', [('Context-Type', 'text/plain')])
        return 'hello world1'
    
    @classmethod
    def create_resource(cls):
        return cls()


                    
    
