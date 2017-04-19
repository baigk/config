from oslo_config import cfg
from wsgi.service import *
import  wsgi.config as config
import logging
import wsgi.Exception as exception

CONF = cfg.CONF

log = logging.getLogger(__name__)

def setup_opts():
    parse_args(sys.argv)

    CONF.register_opt(cfg.ListOpt('proj', default=[], help='proj'))

    for proj in CONF.proj:
        opt = "%s_apps" % proj
        CONF.register_opt(cfg.ListOpt(opt, default=[], help=opt))

def wsgi_setup(proj, launcher):
    apps = getattr(CONF, "%s_apps" % proj, [])

    setup(proj, apps)
    print proj, apps
    for app in apps:
        try:
            service = WSGIService(app, None, 50000)

            launcher.launch_service(service, 1)
        except exception.PasteAppNotFound as ex:
            log.error("%s wsgi_apps %s includes bad values. Fix to remove this warning." % (app, ex))
            raise
        except Exception as ex:
            log.error("launch app %s fail %s." % (app, ex))
            raise

def wsgi_init():
    launcher = process_launcher()
    for proj in CONF.proj:
        wsgi_setup(proj, launcher)
    launcher.wait()

if __name__=="__main__":
    setup_opts()
    wsgi_init()
