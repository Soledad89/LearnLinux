#!/bin/bash
if ! grep "^#\!" $1 &> /dev/null; then
	cat >> $1 << EOFA
#!/bin/bash
# Author
#	wangbo, wangboshine@126.com
# Date & Time
#	`date +"%F %T"`
# Description:
# 
EOFA
fi
vim +7 $1
