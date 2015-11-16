#!/bin/bash
echo "fuck you" #echo
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

echo
echo 'usage for $# $@ $*'
echo '$# vaule is :' $#
echo '$@ value is :' "$@" 
echo '$* value is :' "$*" 
echo

echo "test usage: "
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

exit
