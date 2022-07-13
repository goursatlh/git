#! /bin/bash
#set -e
if [ $# -ne 4 ];then
    echo "error input, Usage: $0 on/off function_name type 0/1(stack enable/disable)"
    exit -1
fi

if [ $1 = "on" ];then
    if [ ! -d /debug ] || [ ! -d /debug/tracing ];then
        echo "mount the /debug filesystem"
        mkdir -p /debug
        mount -t debugfs nodev /debug
    fi
    echo 1 > /proc/sys/kernel/ftrace_enabled

    cd /debug/tracing/
   
    if [ $2 = 1 ];then
        echo function > current_tracer
        if [ -n $3 ];then
            echo $3 > set_ftrace_filter
        fi
        if [ $4 = 1 ];then
            echo 1 > options/func_stack_trace
        fi
    fi
    
    if [ $2 = 2 ];then
        echo function_graph > current_tracer
        if [ -n $3 ];then
            echo $3 > set_graph_function
        fi
        echo $4 > max_graph_depth
        if [ $4 != 1 ];then
            echo $4 > max_graph_depth
        fi
    fi
    
    echo "ftrace start: "
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
    echo 0 > options/func_stack_trace
    echo >trace
    #cd -
    echo "ftrace end"
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


