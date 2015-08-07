#!/bin/bash
#set -x

function destroy_vm()
{
    virsh destroy $1
    virsh undefine $1
    rm -f $1.img
}

function create_vm()
{
    virt-clone -o vm1 -n $1 -f $1.img
    virsh dumpxml $1 > $1.xml
    virsh start $1
}

function query_vm()
{
    get_vm_ip $1
}

function get_vm_ip()
{
   MAC_ADDR=$(virsh dumpxml $1 | sed -n '/mac address/p' | awk -F"[']" '{print $2}')
   IPV4_ADDR=$(arp -ni mgmt | grep ${MAC_ADDR} | awk '{printf $1}')
   if [ -n $IPV4_ADDR ]; then
       fping -u 192.168.50.{1..254} > /dev/null 2>&1
       IPV4_ADDR=$(arp -ni mgmt | grep ${MAC_ADDR} | awk '{printf $1}')
   fi
   
   echo vm:$1 ip:$IPV4_ADDR
}

while getopts "c:q:d:" arg; do
    case $arg in
        c)
            create_vm $OPTARG
            ;;
        q)
            query_vm $OPTARG
            ;;
        d) 
            destroy_vm $OPTARG
            ;;
        *)
            echo "parameter wrong"
            ;;
    esac
done 
