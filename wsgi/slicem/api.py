from util.wsgi import APIRouter
import example.TestControl1
import example.TestControl2

class APIRoute(APIRouter):

    def _setup_routes(self):
        self.resources['TescControl1'] = TestControl1.control.create_resource()

        self.mapper.connect("/test", controller=self.resources['TescControl1'], action="show", conditon={}) 

        self.resources['TescControl2'] = TestControl2.control.create_resource()

        self.mapper.resource("test", "tests", controller=self.resources['TescControl2'], 
                             path_prefix='', 
                             collection = {'show': 'GET', 'version':'get'})
    
