import logging
import abc

log = logging.getLogger(__name__)

class interface():
    __metaclass__ = abc.ABCMeta

    @abc.abstractmethod
    def reg(self, node = None):
        pass

    @abc.abstractmethod
    def get(self, name = None):
        pass

    @abc.abstractmethod
    def update(self,  args):
        pass



class register_interface():
    @abc.abstractmethod
    def reg(self, name, service_id=None, address=None, port=None, tags=None, check=None, token=None, script=None, interval=None, ttl=None, http=None, timeout=None):
       pass

class delegate_interface:
    @abc.abstractmethod
    def get(self, name = None):
        pass

    @abc.abstractmethod
    def update(self, args):
        pass

class service_discover(interface):
    def __init__(self, register, delegate):
        self.register = register
        self.delegate = delegate


    def reg(self, name, service_id=None, address=None, port=None, tags=None, check=None, token=None, script=None, interval=None, ttl=None, http=None, timeout=None):
        return self.register.reg(name, service_id, address, port, tags, check, token, script, interval, ttl, http, timeout)

    def get(self, name = None):
        return self.delegate.get(name)


    def update(self, args):
        return self.delegate.update(args)
