all: philosophes philosophes2 philosophes3 prod-cons prod-cons2 prod-cons3 lecteur-écrivain lecteur-écrivain2 lecteur-écrivain3 tache22 tache23

philosophes: philosophe.c
	echo "philosophe1 exécuté"
	gcc -pthread philosophe.c -o philosophe.o

prod-cons: prod-cons.c
	echo "Prod-conso1 exécuté"
	gcc -pthread prod-cons.c -o prod-cons.o

lecteur-écrivain: lecteur-écrivain.c
	echo "lecteur-écrivain1 exécuté"
	gcc -pthread lecteur-écrivain.c -o lecteur-écrivain.o

philosophes2: philosophe2.c verrou.h
	echo "philosophes2 exécuté"
	gcc -pthread philosophe2.c verrou.c -o philosophe2.o

prod-cons2: prod-cons2.c verrou.h
	echo "Prod-conso2 exécuté"
	gcc -pthread prod-cons2.c verrou.c -o prod-cons2.o

lecteur-écrivain2: lecteur-écrivain2.c verrou.h
	echo "lecteur-écrivain2 exécuté"
	gcc -pthread lecteur-écrivain2.c verrou.c -o lecteur-écrivain2.o

tache22: tâche_and_set.c verrou.h
	echo "tache22 (test test&set) exécuté"
	gcc -pthread tâche_and_set.c verrou.c -o tâche_and_set.o

tache23: tâche_and_test_and_set.c verrou.h
	echo "tache23 (test test&set) exécuté"
	gcc -pthread tâche_and_test_and_set.c verrou.c -o tâche_and_test_and_set.o

philosophes3: philosophe3.c verrou.h
	echo "philosophes3 exécuté"
	gcc -pthread philosophe3.c verrou.c -o philosophe3.o

prod-cons3: prod-cons3.c verrou.h
	echo "Prod-conso3 exécuté"
	gcc -pthread prod-cons3.c verrou.c -o prod-cons3.o

lecteur-écrivain3: lecteur-écrivain3.c verrou.h
		echo "lecteur-écrivain3 exécuté"
		gcc -pthread lecteur-écrivain3.c verrou.c -o lecteur-écrivain3.o

test:
	./experiments.sh
	python3 mesures/plot_philo.py
	python3 mesures/plot_prodcons.py
	python3 mesures/plot_lecteur.py
	python3 mesures/plot_tache223.py

clean:
	rm -f *.o
	rm -f *.csv
