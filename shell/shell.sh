#!/bin/bash
CMD="\033[1;32mUsage for: \033[0m"

echo -e ${CMD}"process opt"
echo "PID: " "$$"
if [ ! -e test ];then         
    dd if=/dev/zero of=test bs=1MB count=1000
fi

if [ ! -e test1 ];then
    dd if=/dev/zero of=test1 bs=1MB count=1000
fi

start=$(date +%s) 
a=()
for var in test test1
do
    md5sum $var &
    a+=($!)
done
wait ${a[@]}
end=$(date +%s)
timespend=$(( end - start ))
echo total time spends parallel $timespend

start=$(date +%s) 
md5sum test test1
end=$(date +%s) 
timespend=$(( end - start ))
echo total time spends $timespend

if [ -e test ];then
    rm -f test
fi

if [ -e test1 ];then
    rm -f test1
fi

no_exec1() {
echo -e ${CMD}"array opt"
array=(1 2 3 4)
for var in ${array[@]}
do
    echo $var
done
echo ${#array[@]} 
echo ${array[@]} 
echo ${array[0]} 

echo "array_b"
array_b=()
i=0
while [ $i -lt 10 ]
do
    array_b+=($i)
    let i++
done
echo "num: "${#array_b[@]} vaule: ${array_b[@]} 

while [ $(( i-- )) -gt 0 ]
do
    array_b+=($i)
done
echo "num: "${#array_b[@]} "value: "${array_b[@]}

echo -e ${CMD}"read"
read str       
echo "you input: " $str 
if [[ $str == *[!0-9]* ]]; then
    echo "Not a number!"
fi

function xyz()
{
    echo 'inside func $0: ' $0
    echo 'inside func $1: ' $1
    echo 'inside func $2: ' $2
    echo 'inside func $3: ' $3
    echo '$10 value is :' ${10}
    echo '$# vaule is :' $#
    echo '$@ vaule is :' "$@"
    echo '$* vaule is :' "$*"
    echo '$$ vaule is :' $$
    shift 2
    echo 'inside func $1 after shift : ' $1
    echo '$# vaule is after shift:' $#
}

xyz fuck you then fuck you then "fuck you then" fuck


echo
echo -e ${CMD}'$# $@ $*'
echo '$# vaule is :' $#
echo '$@ value is :' "$@" 
echo '$* value is :' "$*"

my_fun() {
    echo "$#"
}
echo 'the number of parameter in "$@" is '$(my_fun "$@")
echo 'the number of parameter in "$*" is '$(my_fun "$*")
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
    echo -n $var" "
done
echo

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

echo $((16#3f))
echo $((8#30))

echo -e ${CMD}'printf & echo'
printf "%-10s %-010d %-10.3f\n" hello 1 3.1415926 world 2 4.5678 
printf "%010s %010d %010.3f\n" hello 1 3.1415926 world 2 4.5678 

echo -e "1\t2\t"
echo -n "1\t2\t"
echo "hello"
echo -en "hello\n"

#Colors are represented by color codes, some examples being, reset = 0, black = 30, red = 31, 
#green = 32, yellow = 33, blue = 34, magenta = 35, cyan = 36, and white = 37. 
echo -e "\e[45;37m This is red text \e[0m"
#For a colored background, reset = 0, black = 40, red = 41, green = 42, yellow = 43, blue = 44, 
#magenta = 45, cyan = 46, and white=47, are the color codes that are commonly used.
echo -e "\e[1;42m Green Background \e[0m" 

echo -e ${CMD}'regex'
echo "abc" |grep -E "ab." # www.regexper.com perfect url
echo

echo;echo -e ${CMD}'sed'
echo thisthisthis | sed 's/this/THIS/3g'
tmpfile=test$$
touch $tmpfile;echo thisthisthis>$tmpfile;echo
sed s/this/This/g $tmpfile;cat $tmpfile
sed -i s/this/This/g $tmpfile;cat $tmpfile
rm -f $tmpfile 
echo

}
