from util.wsgi import APIRouter
import control

class APIRoute(APIRouter):
#    def __call__(self, env, req):
#        req('200 OK', [('Conten-Type', 'text/plain')])
#        return 'hello world'

    def _setup_routes(self):
        self.resources['test'] = control.control.create_resource()

        self.mapper.connect(None, "/", controller=self.resources['test'], action="show") 
    
 #   @classmethod
 #   def factory(cls, global_config, **local_config):
 #       return cls()    
