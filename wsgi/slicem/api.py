from util.wsgi import APIRouter
from example.TestController1 import *
from example.TestController2 import *

class APIRoute(APIRouter):

    def _setup_routes(self):
        self.resources['TescControl1'] = TestController1.create_resource()

        self.mapper.connect(None, '/test', controller=self.resources['TescControl1'], action="show", conditon={}) 

        self.resources['TescControl2'] = TestController2.create_resource()

        self.mapper.resource("test", "tests", controller=self.resources['TescControl2'], 
                             path_prefix='', 
                             collection = {'show': 'GET', 'version':'get'})
    
