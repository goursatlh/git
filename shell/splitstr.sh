#!/bin/bash
string=$PATH                  
echo $string
OLD_IFS=$IFS
IFS=":"
array=($string)
IFS=${OLD_IFS}
for var in ${array[@]}
do
    result=$(echo $var | grep "python3")
    if [[ $result = "" ]];then    
        newstr=$newstr${var}":"
    fi
done
echo $newstr
