from discover.service import service_discover
from register import *
from delegate import *
import consul

class std_agent(service_discover):
    _inst = None

    @staticmethod
    def instance():
        if not std_agent._inst:
            std_agent._inst = std_agent(register(consul.Consul()), delegate())

        return std_agent._inst

