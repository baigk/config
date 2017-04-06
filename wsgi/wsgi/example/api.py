from wsgi.wsgi import APIRouter
from wsgi.serializers import *
from wsgi.example.TestController1 import *
from wsgi.example.TestController2 import *

class APIRoute(APIRouter):

    def _setup_routes(self):
        self.resources['TescControl1'] = TestController1.create_resource()

        self.mapper.connect(None, '/test', controller=self.resources['TescControl1'], action="show") 

        self.resources['TescControl2'] = TestController2.create_resource(JSONRequestDeserializer(), JSONResponseSerializer())

        self.mapper.resource("test", "tests", controller=self.resources['TescControl2'], 
                             collection = {'show': 'POST', 'version':'get'})
    
