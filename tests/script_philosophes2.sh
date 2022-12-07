#!/usr/bin/bash

make philosophes2 -s

rm -f mesures/pilo2.csv
touch mesures/philo2.csv
echo "nthreads,time1,time2,time3,time4,time5" >> mesures/philo2.csv
echo "nthreads,time1,time2,time3,time4,time5"

NT=$(nproc)
for((i=2; i<=NT*2; i*=2))
do
 echo -n "$i" >> mesures/philo2.csv
 echo -n "$i"
 for j in {1..5}
 do
  t=$(/usr/bin/time -f %e ./philosophe2.o $i 2>&1 | tail -n 1)
  echo -n ",$t" >> mesures/philo2.csv
  echo -n ",$t"
 done
 echo "" >> mesures/philo2.csv
 echo ""
done

make clean -s 2> /dev/null
