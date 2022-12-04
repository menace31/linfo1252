all: philosophes

philosophes: philosophe.c
	echo "philosophe exécuté"
	gcc -pthread philosophe.c -o philosophe.o

test: 
	./script.sh
	python3 mesure.py

clean: 
	rm -f *.o
	rm -f *.csv