#!/usr/bin/bash

make tache22 -s

rm -f mesures/tache22.csv
touch mesures/tache22.csv
echo "nthreads,time1,time2,time3,time4,time5" >> mesures/tache22.csv
echo "nthreads,time1,time2,time3,time4,time5"

NT=$(nproc)
for((i=1; i<=2*NT; i*=2))
do
 echo -n "1,$i" >> mesures/tache22.csv
 echo -n "1,$i"
 for j in {1..5}
 do
  t=$(/usr/bin/time -f %e ./tâche_and_set.o $i 2>&1 | tail -n 1)
  echo -n ",$t" >> mesures/tache22.csv
  echo -n ",$t"
 done
 echo "" >> mesures/tache22.csv
 echo ""
done

make clean -s 2> /dev/null
