#!/usr/bin/bash

make lecteur-écrivain3 -s

rm -f mesures/lect-ecriv3.csv
touch mesures/lect-ecriv3.csv
echo "nthreads,time1,time2,time3,time4,time5" >> mesures/lect-ecriv3.csv
echo "nthreads,time1,time2,time3,time4,time5"

NT=$(nproc)
for((i=1; i<=NT; i*=2))
do
 Itot=$((2*i))
 echo -n "$Itot" >> mesures/lect-ecriv3.csv
 echo -n "$Itot"
 for j in {1..5}
 do
  t=$(/usr/bin/time -f %e ./lecteur-écrivain3.o $i $i 2>&1 | tail -n 1)
  echo -n ",$t" >> mesures/lect-ecriv3.csv
  echo -n ",$t"
 done
 echo "" >> mesures/lect-ecriv3.csv
 echo ""
done

make clean -s 2> /dev/null
