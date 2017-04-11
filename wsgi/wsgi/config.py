from oslo_config import cfg
import logging
import sys

CONF = cfg.CONF

log_group = cfg.OptGroup('LOG')

wsgi_opts = [
    cfg.StrOpt('project', default='sdme', help='project'),
    cfg.StrOpt('paste_ini', default='paste.ini', help='paste_ini'),
    cfg.StrOpt('listen', help='listen'),
    cfg.IntOpt('listen_port', help='listen_port'),
]
apps_opts = [
    cfg.ListOpt('wsgi_apps', default=[], help='wsgi_apps'),
]

log_opts = [
    cfg.StrOpt('level', default='INFO', help='level'),
]

cfg.CONF.register_group(log_group)


LOG = logging.getLogger(__name__)

def list_opts():
    yield None, wsgi_opts
    yield None, apps_opts
    yield log_group.name, log_opts

for name, opts in list_opts():
    cfg.CONF.register_opts(opts, group=name)


CONF = cfg.CONF

def get_config_value(item, section=None):
    if section is not None:
        if hasattr(CONF, section):
            sec = getattr(CONF, section)
            if hasattr(sec, item):
                para = getattr(sec, item)
                if para is not None:
                    return para
 
    return getattr(CONF, item)

def parse_args(argv, proj='default', ver='1.0'):
    CONF(argv[1:], project = proj, version=ver)

logging.basicConfig(filename= './%s.log' % CONF.project, level=getattr(logging, get_config_value('level', 'LOG'), logging.DEBUG))
def setup(proj="default"):
    for app in CONF.wsgi_apps:
        group = cfg.OptGroup(app)
        cfg.CONF.register_group(group)
        cfg.CONF.register_opts(wsgi_opts, group=app)

