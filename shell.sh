#!/bin/bash
function xyz()
{
    echo 'inside func $1: ' $1
    echo 'inside func $2: ' $2
    echo 'inside func $3: ' $3
    echo '$10 value is :' ${10}
    echo '$# vaule is :' $#
    shift 2
    echo 'inside func $1 after shift : ' $1
    echo '$# vaule is after shift:' $#
}

xyz fuck you then fuck you then fuck you then fuck

CMD="\033[1;32mUsage for: \033[0m"  

echo
echo -e ${CMD}'$# $@ $*'
echo '$# vaule is :' $#
echo '$@ value is :' "$@" 
echo '$* value is :' "$*" 
echo

echo -e ${CMD}"test and if"
A="hello"
B=1

[ $A="hello" ]
echo $?

#if test $A = "hello"
if [ $A = "hello" ]
then 
    echo "string equire"
fi

if [ $B -eq 01 ] 
then
    echo "int equire"
fi

if [ $A = "hello" -a $B -eq 2 ] ; then
    echo "equire"
else
    echo "not equire"
fi

if [ $A = "hello" ] && [ $B -eq 1 ] ; then
    echo "equire 2"
fi

touch txt
if [ -e txt ] ; then
    echo "file exist"
else
    echo "file doesn't exist"
fi

echo
echo -e ${CMD}"case"
case $1 in 
    "hello")
        echo "hello"
        ;;
    "world"):
        echo "world"
        ;;
    *)
        echo "none match"
        ;;
esac

echo
echo -e ${CMD}"loop"
for var in one two three four five
do
    echo '$var is '$var
done

for var1
do
    echo $var1
done

i=0
while [ $i -le 10 ]
do
    echo -n "$i "
    #i=$(($i + 1)) #why add two ()
    ((i = i + 1))
done
echo

echo
echo -e ${CMD}"add opt"
n=1;echo -n "$n "
 
let "n = $n + 1"
echo -n "$n "
 
: $((n = $n + 1))
echo -n "$n "
 
(( n = n +1 ))
echo -n "$n "
 
: $[ n = $n +1 ]
echo -n "$n "
 
n=$[ $n + 1 ]
echo -n "$n "
 
let "n++"
echo -n "$n "
 
(( n++ ))
echo -n "$n "
 
: $[ n++ ]
echo -n "$n "
echo

echo
echo -e ${CMD}"unset"
str=
var=${str=expr}
echo $var
echo $str
echo "newline"
unset str
var=${str=expr}
echo $var
echo $str

echo
echo -e ${CMD}"{}/()"
echo $PWD
(
    cd IPC
    echo ${PWD}" ()"
)
echo $PWD
echo
{
    cd IPC
    echo ${PWD}" {}"
}
echo $PWD
cd ..
echo $PWD


echo
echo -e ${CMD}"source/exec/fork"
touch 2.sh
chmod 777 2.sh
echo '#!/bin/bash
echo "PID for 2.sh: $$"
echo "2.sh get \$A=$A from 1.sh"
A=C
export A
echo "2.sh: \$A is $A"' > 2.sh

A=B
echo "PID for 1.sh before exec/source/fork:$$"
export A
echo "1.sh: \$A is $A"
case $1 in
    exec)
        echo "using exec..."
        exec ./2.sh ;;
    source)
        echo "using source..."
        . ./2.sh ;;
    *)
        echo "using fork by default..."
        ./2.sh ;;
esac
echo "PID for 1.sh after exec/source/fork:$$"
echo "1.sh: \$A is $A"

if [ -e 2.sh ];then
    rm -f 2.sh
fi

echo
echo -e ${CMD}'$()/${}'
file=/dir1/dir2/dir3/my.file.txt
echo '${#file} '${#file}



exit
