import multiprocessing
import logging
import signal
import time
import os

LOG = logging.getLogger(__name__)


class process():
    _instance = None
    @staticmethod
    def term(sig_num, addtion):
        print 'terminate process %d' % os.getpid()
        try:
            for name, p in process.getinstance().processes.items():
                print 'process %d terminate' % p.pid
                #p.terminate()
                os.kill(p.pid, signal.SIGKILL)
        except Exception as e:
            LOG.exception("%s",  e)
            print str(e)
  
    def __init__(self):
        self.processes = {}
        signal.signal(signal.SIGTERM, process.term)

    @staticmethod
    def getinstance():
        if process._instance is None:
            process._instance = process()

        return process._instance

    def create(self, name, func, *args, **kwargs):
        p = multiprocessing.Process(target = func, args = (args, kwargs))
        self.processes.update({name:p})
        print self.processes
        p.start()
        LOG.info("%s(%d) start ...", name, p.pid)

    def join(self):
        try:
            for name, p in sefl.processes.items():
                print p.pid, name
                p.join()
        except Exception as e:
            LOG.exception("exit", e)


def test():
     def hello(*args, **kwargs):
         while True:
             time.sleep(1)   

     process.getinstance().create("test1", hello)
     process.getinstance().create("test2", hello)
     process.getinstance().create("test3", hello)

     print process.getinstance().processes
     process.getinstance().join()

if __name__ == "__main__":
    test()
        
