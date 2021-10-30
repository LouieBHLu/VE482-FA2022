#!/bin/bash
out=./ex3_naive

if ! test -f "$out"; then
    echo "0" >> $out
fi

for i in {1..50}
do
    operand=$(tail -n 1 $out)
    ((operand++))
    echo $operand >> $out
done
