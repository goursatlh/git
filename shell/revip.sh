#!/bin/bash
IP=$1
IFS="."
TMPIP=$(echo $IP)
echo $TMPIP
IFS=" "
for x in $TMPIP
do
    Xip="${x}.$Xip"
done
echo ${Xip%.}
