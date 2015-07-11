#!/bin/bash
# Author:wangbo, wangboshine@126.com
#Date & Time: 2015-07-11 10:24:24
#Description:
#
FILE="/etc/passwd"
num=$(wc -l < $FILE)
echo "User count:$num"
for i in $(seq 1 $num);
do
	Username=$(head -$i $FILE | tail -1 | cut -d: -f1)
	Shell=$(head -$i $FILE | tail -1 | cut -d: -f7)
	echo "Hello,$Username,your shell: $Shell"
done 
