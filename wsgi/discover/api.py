from wsgi.wsgi import APIRouter
from wsgi.serializers import *
from discover.ServiceController import *


class APIRoute(APIRouter):

    def _setup_routes(self):
        self.resources['ServiceController'] = ServiceController.create_resource()

        self.mapper.connect(None, '/health', controller=self.resources['ServiceController'], action="health", conditions={'method' : ["GET"]})

        self.mapper.connect(None, '/', controller=self.resources['ServiceController'], action="get", conditions={'method' : ["GET"]})

        self.mapper.connect(None, '/', controller=self.resources['ServiceController'], action="update", conditions={'method' : ["POST"]})

