all: philosophes prod-cons lecteur-écrivain philosophes2 prod-cons2 lecteur-écrivain2 tache22

philosophes: philosophe.c
	echo "philosophe exécuté"
	gcc -pthread philosophe.c -o philosophe.o

prod-cons: prod-cons.c
	echo "Prod-conso exécuté"
	gcc -pthread prod-cons.c -o prod-cons.o

lecteur-écrivain: lecteur-écrivain.c
	echo "lecteur-écrivain exécuté"
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

tache22: tache22.c verrou.h
	echo "tache22 (test test&set) exécuté"
	gcc -pthread tache22.c verrou.c -o tache22.o

test:
	./script.sh
	python3 mesure.py

clean:
	rm -f *.o
	rm -f *.csv
