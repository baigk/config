from discover.service import *

class register(register_interface):
    def __init__(self, agent):
        self.agent = agent

    def reg(self, name, service_id=None, address=None, port=None, tags=None, check=None, token=None, script=None, interval=None, ttl=None, http=None, timeout=None):
        self.agent.agent.service.register(name, service_id, address, port, tags, check, token, script, interval, ttl, http, timeout)

        print name, service_id, address, port, tags, check, token, script, interval, ttl, http, timeout


