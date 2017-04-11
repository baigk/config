from oslo_config import cfg
import logging
import sys

CONF = cfg.CONF

CONF(sys.argv[1:], version='1')

log_group = cfg.OptGroup('LOG')

services_opts = [
    cfg.StrOpt('paste_ini', default=None, help='paste_ini'),
    cfg.StrOpt('listen', default='127.0.0.1', help='slicem_listen'),
    cfg.IntOpt('listen_port', default=8888, help='slicem_listen_port'),
]

log_opts = [
    cfg.StrOpt('level', default='INFO', help='level'),
]

cfg.CONF.register_group(log_group)
cfg.CONF.register_group(cfg.OptGroup('app1'))

logging.basicConfig(filename='slicem.log', level=logging.DEBUG)

LOG = logging.getLogger(__name__) 
         
def list_opts():
    yield None, services_opts
    yield 'app1', services_opts
    yield log_group.name, log_opts

for name, opts in list_opts(): 
    cfg.CONF.register_opts(opts, group=name)

def get_cfg_value(item, section=None):
    if section is None:
        return getattr(CONF, item)

    sec = getattr(CONF, section)    
    print sec, section, item
    return getattr(sec, item)
     

print CONF.listen
print CONF.app1.listen

print get_cfg_value('listen')
print get_cfg_value('listen', 'app1')
