#!/usr/bin/bash

make tache23 -s

rm -f mesures/tache23.csv
touch mesures/tache23.csv
echo "nthreads,time1,time2,time3,time4,time5" >> mesures/tache23.csv
echo "nthreads,time1,time2,time3,time4,time5"

NT=$(nproc)
for((i=1; i<=2*NT; i*=2))
do
 echo -n "$i" >> mesures/tache23.csv
 echo -n "$i"
 for j in {1..5}
 do
  t=$(/usr/bin/time -f %e ./tâche_and_test_and_set.o $i 2>&1 | tail -n 1)
  echo -n ",$t" >> mesures/tache23.csv
  echo -n ",$t"
 done
 echo "" >> mesures/tache23.csv
 echo ""
done

make clean -s 2> /dev/null
