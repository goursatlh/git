#! /bin/bash 
set -x
echo module_name: $1
module_addr=$(cat /proc/kallsyms |grep $1|grep __this_module|awk '{print $1}')
insmod force_rmmod.ko addr=0x$module_addr
sleep 1
rmmod $1
sleep 1
rmmod force_rmmod
set +x
