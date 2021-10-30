#!/bin/bash
for i in {1..20}
do
    # ./ex3_naive.sh; ./ex3_naive.sh&
    ./ex3_revised.sh; ./ex3_revised.sh&
done
