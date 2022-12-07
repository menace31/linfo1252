#!/usr/bin/bash

make prod-cons -s

rm -f mesures/prod-cons1.csv
touch mesures/prod-cons1.csv
echo "nthreads,time1,time2,time3,time4,time5" >> mesures/prod-cons1.csv
echo "nthreads,time1,time2,time3,time4,time5"

NT=$(nproc)
for((i=1; i<=32; i*=2))
do
 Itot=$((2*i))
 echo -n "1,$Itot" >> mesures/prod-cons1.csv
 echo -n "1,$Itot"
 for j in {1..5}
 do
  t=$(/usr/bin/time -f %e ./prod-cons.o $i $i 2>&1 | tail -n 1)
  echo -n ",$t" >> mesures/prod-cons1.csv
  echo -n ",$t"
 done
 echo "" >> mesures/prod-cons1.csv
 echo ""
done

make clean -s 2> /dev/null
