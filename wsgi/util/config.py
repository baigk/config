from oslo_config import cfg
CONF = cfg.CONF

def parse_args(argv, proj='default', ver='1.0'):
    CONF(argv[1:], project = proj, version=ver)

