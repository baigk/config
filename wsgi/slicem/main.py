
import sys
from oslo_config import cfg
from util import config
from util.service import WSGIService
import logging

CONF = cfg.CONF

test_group = cfg.OptGroup('test_group')

services_opts = [
    cfg.StrOpt('paste_ini', default=None, help='paste_ini'),
    cfg.StrOpt('slicem_listen', default='127.0.0.1', help='slicem_listen'),
    cfg.IntOpt('slicem_listen_port', default=8888, help='slicem_listen_port'),
]

test_opts = [
    cfg.StrOpt('test', default='test', help='test'),
]

cfg.CONF.register_group(test_group)

logging.basicConfig(filename='slicem.log', level=logging.INFO)

log = logging.getLogger(__name__)

def list_opts():
    yield None, services_opts
    yield test_group.name, test_opts

for name, opts in list_opts():
    cfg.CONF.register_opts(opts, group=name)

def main():
    config.parse_args(sys.argv, "slicem")


def hello(env, resp):
    print env
    resp('200 OK', [('Content-Type', 'text/plain')])
    return 'hello world\n'

class test_load():
    def load_app(self, name):
	return hello

def test():
    config.parse_args(sys.argv, "slicem")
    s = WSGIService('slicem', None, 50000)
    s.start()
    s.wait()
