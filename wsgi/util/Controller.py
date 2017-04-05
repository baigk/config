import serializers
import wsgi

class Controller(object):   
    @classmethod
    def create_resource(cls):
       return wsgi.Resource(cls())

       
class SeriallizerController(object):   
    @classmethod
    def create_resource(cls):
       deserializer = wsgi.JSONRequestDeserializer()
       serializer = serializers.JSONResponseSerializer()
       return wsgi.Resource(cls(), deserializer, serializer)

                    