#!/bin/sh
######################################################################################
#���ã���ָ���ļ���Ŀ¼�µ�����.c��.h�ļ�ͳһ��indent�Ķ�������д����ʽ��
#######################################################################################

if [ $# != 1 ]; then
    echo "bad input : indent filename or directory name"
    exit
fi


echo "indent $1 start"
if [ -d "$1" ]; then
    find $1 -name "*.c" |xargs indent -bad -bap -nbc -bbo -hnl -bl -bls -c33 -cd33 -ncdb -nce -ci4 -cli0 -d0 -di1 -nfc1 -i8 -ip0 -l80 -lp -npcs -nprs -npsl -ncs -nsc -nsob -nfca -cp33 -ss -ts8 -bli 0
    find $1 -name "*.h" |xargs indent -bad -bap -nbc -bbo -hnl -bl -bls -c33 -cd33 -ncdb -nce -ci4 -cli0 -d0 -di1 -nfc1 -i8 -ip0 -l80 -lp -npcs -nprs -npsl -ncs -nsc -nsob -nfca -cp33 -ss -ts8 -bli 0
fi

if [ -f "$1" ]; then
    indent -bad -bap -nbc -bbo -hnl -bl -bls -c33 -cd33 -ncdb -nce -ci4 -cli0 -d0 -di1 -nfc1 -i8 -ip0 -l80 -lp -npcs -nprs -npsl -ncs -nsc -nsob -nfca -cp33 -ss -ts8 -bli 0 $1
fi
echo "indent $1 end"


