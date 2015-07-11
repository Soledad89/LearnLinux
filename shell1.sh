#!/bin/bash
#Program:
#	User input dir name, I find the permission of files
#History:
#	2015/07/10 wangbo, first release, v1.0

# Show the earthquakes and store it. 
curl http://earthquake.usgs.gov/earthquakes/feed/v1.0/summary/1.0_month.csv \
	--silent | sed '1d' | cut -d "," -f 5 > earthquakes.cvs1

read -p "Please input a directory:" dir
if [ "$dir" == "" -o ! -d "$dir" ];then
	echo "The $dir is NOT exist in your system"
	exit 1
fi

filelist=$(ls $dir)
for filename in $filelist
do
	perm=""
	test -r "$dir/$filename" && perm="$perm readable"
	test -w "$dir/$filename" && perm="$perm writable"
	test -x "$dir/$filename" && perm="$perm executable"
	echo "The file $dir/$filename's permission is $perm"
done

