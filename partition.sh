#!/bin/bash
vgname=host1
fdisk -l
pvcreate /dev/sdb
vgextend $vgname /dev/sdb
lvresize /dev/host2/homevol -L +300G
resize2fs /dev/mapper/host2-homevol

