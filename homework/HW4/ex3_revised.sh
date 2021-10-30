#!/bin/bash
out=./ex3_revised
if ! test -f "$out";
then
    echo "0" >> $out
fi

for i in {1..50}
do
(
    flock -n -x 33
    if [ $? -eq 1 ];
    then
        exit;
    fi
    operand=$(tail -n 1 $out)
    ((operand++))
    echo $operand >> $out
) 33>>$out
done
