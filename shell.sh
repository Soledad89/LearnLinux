#!/bin/bash

# This program shows how to use shell with examples
us=$(grep -v '^#' /etc/passwd | cut -d ':' -f2)
for user in $us
do
	echo $user
done

for animal in dog cat elephant
do
	echo "There are ${animal}s...."
done

read -p "Please input (Y/N):" yn
if [ "$yn" == "Y" ] || [ "$yn" == "y" ]; then
	echo "OK, continue"
	exit 0
elif [ "$yn" == "N" ] || [ "$yn" == "n" ]; then
	echo "OK, interrupt"
	exit 0
fi

echo  "hello, bob!"
read -p "input your firstname: " firstname
read -p "input your lastname : " lastname
echo "your name is: $firstname $lastname"

echo -e "I will usr 'touch' command to create 3 files."
read -p "Please inpurt your name:" fileuser

filename=${fileuser:-"filename"}
date1=$(date)
echo $date1

echo "The script name is: $0"
echo "The parameter number is: $#"
[ "$#" -lt 2 ] && echo "The number of parameter is less than 2. Stop here."\
	&& exit 0

echo "your whole parameter is '$@'"
echo "The 1st parameter is $1"
echo "The 2nd parameter is $2"

exit 0
