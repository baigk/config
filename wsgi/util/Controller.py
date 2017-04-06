import wsgi

class Controller(object):   
    @classmethod
    def create_resource(cls, deserializer = None, serializer = None):
       return wsgi.Resource(cls(), deserializer, serializer)

                    
