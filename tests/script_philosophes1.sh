#!/usr/bin/bash

make philosophes -s

rm -f mesures/pilo1.csv
touch mesures/philo1.csv
echo "nthreads,time1,time2,time3,time4,time5" >> mesures/philo1.csv
echo "nthreads,time1,time2,time3,time4,time5"

NT=$(nproc)
for((i=2; i<=64; i*=2))
do
 echo -n "1,$i" >> mesures/philo1.csv
 echo -n "1,$i"
 for j in {1..5}
 do
  t=$(/usr/bin/time -f %e ./philosophe.o $i 2>&1 | tail -n 1)
  echo -n ",$t" >> mesures/philo1.csv
  echo -n ",$t"
 done
 echo "" >> mesures/philo1.csv
 echo ""
done

make clean -s 2> /dev/null
