#/bin/bash

read -p "what extendname you want to change: " exname 
read -p "you want to change to: " new
read -p "please tell me the directory: " dir

echo $dir
cd $dir
for file in $(ls) 
do
    #name=`ls $file | cut -d. -f1`
    mv $file $file.$new
done
