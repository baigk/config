from oslo_config import cfg
from wsgi.service import *
import  wsgi.config as config
import logging
import wsgi.Exception as exception

CONF = cfg.CONF


def main():
    parse_args(sys.argv)
    setup(get_config_value('project', 'sdme'))
    log = logging.getLogger(__name__)
    
    started = 0

    launcher = process_launcher()
    
    for app in CONF.wsgi_apps:
        try:
            server = WSGIService(app, None, 50000)
               
            launcher.launch_service(server, 1)
            started += 1
        except exception.PasteAppNotFound as ex:
            log.error("%s wsgi_apps %s includes bad values. Fix to remove this warning." % (app, ex))
        except Exception as ex:
            log.error("launch app %s fail %s." % (app, ex))

    if started == 0:
        log.error('No APIs were started. Check the enabled_apis config option.')
        sys.exit(1)
    
    launcher.wait()

if __name__=="__main__":
    main()
