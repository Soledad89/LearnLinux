#!/bin/bash
# Author:wangbo, wangboshine@126.com
#Date & Time: 2015-07-11 10:39:37
#Description:
#
BASH_C=`grep '/bin/bash' /etc/passwd | wc -l`
NOLOGIN_C=`grep '/sbin/nologin' /etc/passwd | wc -l`
echo "BASH,$BASH_C'users',they are:"
for i in `seq 1 $BASH_C`;do
	    BASH_N="$BASH_N`grep '/bin/bash' /etc/passwd | head -$i | tail -1 | cut -d: -f1`,"    
done
echo $BASH_N
echo "NOLOGIN,$NOLOGIN_C'users',they are:"
for i in `seq 1 $NOLOGIN_C`;do
	NOLOGIN_N="$NOLOGIN_N`grep '/sbin/nologin' /etc/passwd | head -$i | tail -1 | cut -d: -f1`,"
done
echo $NOLOGIN_N
