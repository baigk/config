import inspect, sys

class ExceptionBase(Exception):
   
    def __init__(self, message=None):

        self.message = message

        super(ExceptionBase, self).__init__(message)

    def format_message(self):
        return self.args[0]


class ConfigNotFound(ExceptionBase):
    pass

class PasteAppNotFound(ExceptionBase):
    pass

class HTTPExceptionDisguise(Exception):
    def __init__(self, exception):
        self.exc = exception
        self.tb = sys.exc_info()[2]
