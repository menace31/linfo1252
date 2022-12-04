#!/bin/bash

echo "bash execution"

N_run=5
N_thread=64

for file in *.o
do
    echo "thread,time" > ${file%.o}.csv
    for ((i = 1; i <= N_thread; i= i*2))
    do
        for ((j = 0; j < N_run; j++))
        do
            output=$(/usr/bin/time -f %e ./$file $i -s 2>&1)
            echo $i,$output >> ${file%.o}.csv
        done
    done
done
