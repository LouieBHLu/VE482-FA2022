#!/bin/bash
answer=$((($RANDOM)%10))
number=0

while [ $answer -ne $number ] ; do
    read -p "Input an integer: " number
    if [ $answer -eq $number ] ; then
        echo "Congratulation, right answer!"
        break
    elif [ $answer -lt $number ] ; then
        echo "Smaller"
    else
        echo "Larger"
    fi
done
