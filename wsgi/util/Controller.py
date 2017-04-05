import serializers
import wsgi

class Controller(object):   
    @classmethod
    def create_resource(cls):
       return cls()

       
class SeriallizerController(object):   
    @classmethod
    def create_resource(cls):
       deserializer = serializers.JSONRequestDeserializer()
       serializer = serializers.JSONResponseSerializer()
       return wsgi.Resource(cls(), deserializer, serializer)

                    
