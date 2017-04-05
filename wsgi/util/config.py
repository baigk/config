from oslo_config import cfg
import logging

CONF = cfg.CONF


def parse_args(argv, proj='default', ver='1.0'):
    CONF(argv[1:], project = proj, version=ver)

def setup_logging():
	try:
		level = CONF.LOG.level
		logging.getLogger(__name__).setLevel(eval(logging. + level))
	except:
		pass
