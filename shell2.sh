#!/bin/bash

if [ ! -e logical ]; then
	touch logical
	echo "Just make a file logical"
	exit 1
elif [ -e logical ]&&[ -f logical ]; then
	rm logical
	mkdir logical
	echo "remove file ==>> logical"
	echo "and make directory logical"
	exit 1
elif [ -e logical ]&&[ -d logical ];then
	rm -rf logical
	echo "remove directory ==> logical"
	exit 1
else
	echo "Does here have anything"
fi


