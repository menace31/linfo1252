#!/usr/bin/bash

make prod-cons3 -s

rm -f mesures/prod-cons3.csv
touch mesures/prod-cons3.csv
echo "nthreads,time1,time2,time3,time4,time5" >> mesures/prod-cons3.csv
echo "nthreads,time1,time2,time3,time4,time5"

NT=$(nproc)
for((i=1; i<=32; i*=2))
do
 Itot=$((2*i))
 echo -n "3,$Itot"
 echo -n "3,$Itot" >> mesures/prod-cons3.csv
 for j in {1..5}
 do
  t=$(/usr/bin/time -f %e ./prod-cons3.o $i $i 2>&1 | tail -n 1)
  echo -n ",$t"
  echo -n ",$t" >> mesures/prod-cons3.csv
 done
 echo ""
 echo "" >> mesures/prod-cons3.csv
done

make clean -s 2> /dev/null
