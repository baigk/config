#!/usr/bin/python
#coding:utf-8

import sys
from Cheetah.Template import Template

services_list = [  
{ 'consul' : '192.168.134.150' },  
]

services_map = {}

for _ in services_list:
    for k, v in _.items():
        services_map.setdefault(k, []).append(v)


print services_map

nginx = Template(file='nginx.conf.tmpl', searchList = {'services_map' : services_map})

with (open('/etc/nginx/conf.d/nginx.conf', 'w')) as f:
    f.write(nginx.respond())


