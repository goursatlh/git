#! /bin/bash
set -x
#mypid=$(ps aux|grep $1|grep -v 'grep'|awk '{print $2}')
mypid=$(ps aux|grep $1 -m 1|awk '{print $2}')
echo $mypid
cpudir="test-$mypid"
mkdir -p /sys/fs/cgroup/cpu/$cpudir/
echo 50000 > /sys/fs/cgroup/cpu/$cpudir/cpu.cfs_quota_us
echo $mypid > /sys/fs/cgroup/cpu/$cpudir/tasks
set +x
