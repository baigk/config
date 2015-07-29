#!/bin/bash
set -x

device=eth0
bridge=mgmt
function is_brigde_exit()
{
    ret=$(brctl show | grep $bridge | wc -l)
    if [ "$ret" gt "0" ]; then
        /bin/true
    else
        /bin/false
    fi
}

function create_briage()
{
    ips=$(ip addr show $device | grep 'inet ' | awk -F' ' '{print $2}')
    ip addr flush $device
    brctl addbr $bridge
    brctl addif $bridge $device
    ip link set $bridge up
   
    for ip in $ips; do
        ip addr add $ip dev $bridge
    done

    route add default gw 192.168.124.1
}

function is_base_vm_exist()
{
    ret=$(virsh list | awk  '$2=="host1" {print $2}' | wc -l)
    if [ $ret gt 0 ]; then
        /bin/true
    else
        /bin/false
    fi
}

function get_ip_addr()
{
     vm_name=$1
     mac=$(virsh dumpxml $vm_name | grep 'mac address=' | awk -F"'" '{print $2}')
     while :; do
         ip=$(arp -ni $bridge | awk '$3 ~/$mac/{print $1}')
         if [ ! -n $ip ]; then
              sleep 5
              fping 192.168.50.{2..254}
         fi
         echo $ip
         break
     done     
}

function create_vm()
{
    virt-clone -o base -n $1 -f $1.img
    virsh dumpxml $1 > $1.xml
    virsh start $1
    get_ip_addr
}

function delete_vm()
{
   virsh destroy $1
   virsh undefine $1
   rm -rf $1.img $1.xml
}

function main()
{
    if ! is_brigde_exit; then
        create_briage
    fi

    exit 
    
    if ! is_base_vm_exist; then
        qemu-create -f qcow2 base 30G
        virsh define base.xml
        virsh start base
    fi
    
    state=$(virsh list --all | awk '$2 ~/^host5$/{print $3 " " $4}')
    if [ "$state" != "shut off" ]; then
        virsh shutdown base
    fi
    create_vm $1
}
main
