from discover.consul.std_agent import *


if __name__ == "__main__":
    std_agent.instance().reg("test", address="http://192.168.134.150:8000", check={'http':"http://192.168.134.150:8888/health", "internal":"5s"})


