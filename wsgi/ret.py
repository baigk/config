#!/usr/bin/python
#coding:utf-8

import sys
from oslo_config import cfg
from util.http import *
from wsgi.config import *

CONF = cfg.CONF
cfg.CONF.register_group(cfg.OptGroup('discover'))                                      
cfg.CONF.register_opts(wsgi_opts, group='discover')    

parse_args(sys.argv)

service_list = [  
{ 'consul' : '192.168.134.150' },   
{ 'sdme' : '192.168.134.150' },   
{ 'test' : 'http://192.168.134.150:7777/health' },  
]

service_map = {}

for _ in service_list:
    for k, v in _.items():
        service_map.setdefault(k, []).append(v)


print service_map
client = HTTPClient(get_config_value('listen', 'discover'), get_config_value('listen_port', 'discover'))
client.post(callback.null, "", json=service_map)
