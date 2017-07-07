#! /bin/bash
if [ $1 = "on" ];then
    if [ ! -d /debug ];then
        echo "mount the /debug filesystem"
        mount -t debugfs nodev /debug
    fi
    echo 1 > /proc/sys/kernel/ftrace_enabled

    cd /debug/tracing/
    echo function > current_tracer
    
    if [ -n $2 ];then
        echo $2 > set_ftrace_filter
    fi
    echo "start ftrace: "
    echo >trace;echo 1 >tracing_on
    exit
fi

#disable the ftrace and scp the txt to local host
if [ $1 = "off" ];then
    txt=file-$$
    touch $txt
    path=$PWD
    cd /debug/tracing/
    cat trace >$path/$txt
    echo 0 > tracing_on
    echo >trace
    cd -
fi

#tmp=TMP
#cat > $tmp << EOF
#spawn scp $path/$txt  wwang@10.88.200.23:/sview/wwang_sview13_103az_04sz/test/
#expect ":"
#send "Wt19820703@@\n"
#interact
#EOF
#expect -f $tmp
#rm -f $path/$txt


