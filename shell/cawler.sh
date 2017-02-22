#!/bin/bash
echo $#
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

echo $url
echo $directory

mkdir -p $directory
baseurl=$(echo $url | egrep -o "https?://[a-z.]+")
echo $baseurl

#<img src="http://www.chinaunix.net/images_new/login.gif"></img>
echo "Downloading $baseurl"
curl -s $url | egrep -o "<img src=[^>]*>" |
sed 's/<img src=\"\([^"]*\).*/\1/g' > test/$$.list

sed -i "s|^/|$baseurl/|" test/$$.list
cd $directory

while read filename;
do
    echo Downloading $filename
    curl -s -O "$filename" --silent
done < $$.list

#rm -f /tmp/*.list


