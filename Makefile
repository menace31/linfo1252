all: philosophes prod-cons lecteur-écrivain

philosophes: philosophe.c
	echo "philosophe exécuté"
	gcc -pthread philosophe.c -o philosophe.o

prod-cons: prod-cons.c
	echo "Prod-conso exécuté"
	gcc -pthread prod-cons.c -o prod-cons.o

lecteur-écrivain: lecteur-écrivain.c
	echo "lecteur-écrivain exécuté"
	gcc -pthread lecteur-écrivain.c -o lecteur-écrivain.o

test:
	./script.sh
	python3 mesure.py

clean:
	rm -f *.o
	rm -f *.csv
