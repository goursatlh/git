#!/bin/sh
######################################################################################
#���ã��ѵ�ǰĿ¼�µ�����.c��.h�ļ�ͳһ��indent�Ķ�������д����ʽ��
#����: ��ָ��Ŀ¼��ִ��~/.fileformat.sh
#######################################################################################

THIS_CMD=$0
COMMAND=$1

USRPATH=$PWD

echo "THIS_CMD" $THIS_CMD
echo "COMMAND" $COMMAND

#ѭ���ĵ�ÿ��Ŀ¼����ȥ��ÿһ���ļ���^Mɾ��

if [ "$1" = "help" ]; then
    echo reccurrun COMMAND
    echo "    Navigate recursivly the current directory and run COMMAND on"
    echo "     all the files(not directory)."
    exit 0
fi

FILES=`ls`
for file in $FILES;
do
    if [ "$file" = "." ] || [ "$file" = ".." ]; then
        continue
    fi
    if [ -d "$file" ]; then
        TopDir=`pwd`
        cd $file
        #$THIS_CMD $COMMAND
   ls -la |xargs perl -p -i -e "s/[\015]//;"
        cd $TopDir
    fi
    if [ -f "$file" ]; then
        #$COMMAND $file
   perl -p -i -e "s/[\015]//;" $file
    fi
done

#exit

#find $USRPATH -name "*.c" | xargs indent
#find $USRPATH -name "*.h" | xargs indent
find $PWD -name "*.c" |xargs indent -bad -bap -nbc -bbo -hnl -bl -bls -c33 -cd33 -ncdb -nce -ci4 -cli0 -d0 -di1 -nfc1 -i8 -ip0 -l80 -lp -npcs -nprs -npsl -ncs -nsc -nsob -nfca -cp33 -ss -ts8 -bli 0
find $PWD -name "*.h" |xargs indent -bad -bap -nbc -bbo -hnl -bl -bls -c33 -cd33 -ncdb -nce -ci4 -cli0 -d0 -di1 -nfc1 -i8 -ip0 -l80 -lp -npcs -nprs -npsl -ncs -nsc -nsob -nfca -cp33 -ss -ts8 -bli 0
