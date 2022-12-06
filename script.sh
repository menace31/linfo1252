#!/bin/bash

echo "bash execution"

N_run=5
N_thread=64

for file in *.o
do
    echo "thread,time" > ${file%.o}.csv
    for ((i = 2; i <= N_thread; i= i*2))
    do
    	if [ $file = philosophe.o  -o $file = philosophe2.o -o $file = test_and_set.o -o $file = test_and_test_and_set.o ]
        then
            for ((j = 0; j < N_run; j++))
            do
                echo $i
                output=$(/usr/bin/time -f %e ./$file $i -s 2>&1)
                echo $i,$output >> ${file%.o}.csv
            done
        else 
            t1=$(($i / 2))
            t2=$t1
            for ((j = 0; j < N_run; j++))
            do
            echo $t1 + $t2
                output=$(/usr/bin/time -f %e ./$file $t1 $t2 -s 2>&1)
                echo $i,$output >> ${file%.o}.csv
            done
        fi
        
    done
done
