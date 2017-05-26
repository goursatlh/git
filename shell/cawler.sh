#!/bin/bash
if [ $# -ne 3 ];then
    echo "Usage: $0 URL -d DIRECTORY"
    exit -1
fi

#for i in {1..4}
while [ -n "$1" ]
do
    case $1 in
        -d) 
            shift; directory=$1; shift;;
        *) 
            url=${url:-$1}; shift;;
    esac
done


mkdir -p $directory
baseurl=$(echo $url | egrep -o "https?://[a-z.]+")
echo $baseurl

#<img src="http://www.chinaunix.net/images_new/login.gif"></img>
echo "Begin to download gif in $baseurl ......"

curl -s $url | egrep -o "<img src=[^>]*>" |
sed 's/<img src=\"\([^"]*\).*/\1/g' > /tmp/$$.list

sed -i "s|^/|$baseurl/|" /tmp/$$.list
sed -i -e 's/^[ /t]*//g' -e '/^$/d' /tmp/$$.list

cd $directory

while read filename;
do
    echo Downloading $filename
    curl -s -O "$filename" --silent
done < /tmp/$$.list

echo "The End"
#rm -f /tmp/*.list


