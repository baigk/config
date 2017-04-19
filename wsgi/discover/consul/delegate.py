from discover.service import *

class delegate(delegate_interface):
    def __init__(self):
        self.services = {}

    def get(self, name = None):
        if name is None:
            return self.services.copy()
        else:
            return self.services.get(name, []).copy()


    def update(self, args):
        print args
        self.services = args.copy()


