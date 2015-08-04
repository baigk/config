#!/bin/bash
apt-get -y install openipmi ipmitool
modprobe ipmi_msghandler
modprobe ipmi_devintf
modprobe ipmi_si

#query ipmi user
ipmitool user list 1

#set administrator passwort 2 is admin id
ipmitool user set password 2 chenshake

# set ipmi ip
ipmitool lan set 1 ipsrc dhcp 
ipmitool lan print 1

ipmitool lan set 1 ipsrc static

ipmitool lan set 1 ipaddress 10.1.199.211 Setting LAN IP Address to 10.1.199.211

ipmitool lan set 1 netmask 255.255.255.0 Setting LAN Subnet Mask to 255.255.255.0

ipmitool lan set 1 defgw ipaddr 10.1.199.1 Setting LAN Default Gateway IP to 10.1.199.1

ipmitool lan print 1
################################################

#use static ip: ipmitool lan set <channel_no> ipsrc static
#ue dynimic ip: ipmitool lan set <channel_no> ipsrc dhcp
#set ip:        ipmitool lan set <channel_no> ipaddr <x.x.x.x>
#set mask:      ipmitool lan set <channel_no> netmask <x.x.x.x>
#set gw:        ipmitool lan set <channel_no> defgw ipaddr <x.x.x.x>
#local operator -l open is interface local: ipmitool -I open lan print 1
#remote operator -l lan is interface remote: ipmitool -I lan -H 10.1.199.12 -U ADMIN -P ADMIN lan print 1
#############################################################

#change boot seq
ipmitool -I lan -H 10.1.199.212 -U ADMIN -P ADMIN chassis bootdev pxe
ipmitool -I lan -H 10.1.199.212 -U ADMIN -P ADMIN chassis bootdev disk
ipmitool -I lan -H 10.1.199.212 -U ADMIN -P ADMIN chassis bootdev cdrom

# power management
ipmitool -I lan -H 10.1.199.212 -U ADMIN -P ADMIN chassis power off  
ipmitool -I lan -H 10.1.199.212 -U ADMIN -P ADMIN chassis power reset 
ipmitool -I lan -H 10.1.199.212 -U ADMIN -P ADMIN chassis power on   
ipmitool -I lan -H 10.1.199.212 -U ADMIN -P ADMIN chassis power status
