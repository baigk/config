
import sys
from oslo_config import cfg
from util import config
from util.service import WSGIService
import logging

CONF = cfg.CONF

log_group = cfg.OptGroup('LOG')

services_opts = [
    cfg.StrOpt('paste_ini', default=None, help='paste_ini'),
    cfg.StrOpt('slicem_listen', default='127.0.0.1', help='slicem_listen'),
    cfg.IntOpt('slicem_listen_port', default=8888, help='slicem_listen_port'),
]

log_group = [
    cfg.StrOpt('level', default='INFO', help='level'),
]

cfg.CONF.register_group(log_group)

logging.basicConfig(filename='slicem.log', level=logging.INFO)

LOG = logging.getLogger(__name__)

def list_opts():
    yield None, services_opts
    yield log_group.name, log_group

for name, opts in list_opts():
    cfg.CONF.register_opts(opts, group=name)

def main():

	try:
	    config.parse_args(sys.argv, "slicem")
	    setup_logging()
	    
	    s = WSGIService('slicem', None, 50000)
	    s.start()
	    s.wait()

	except Exception: 
        LOG.exception("Failed to start slicem"))



