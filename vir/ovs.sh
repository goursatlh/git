#! /bin/bash
BRIDGE="ovs-br0"
echo ${BRIDGE}
ovs-vsctl add-br ${BRIDGE}
ifconfig ${BRIDGE} 10.0.3.1/24 up
echo $*
if [ $1 -eq 1 ];then
    echo "start my-container"
    lxc-start -n my-container -d --logfile /var/lxclog
else
    echo "stop my-container"
    lxc-stop -n my-container
    ovs-vsctl del-br ${BRIDGE}
fi

