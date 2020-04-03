affichage:
	echo "Si vous voulez complier puis exécuter l'exercice 1 fait 'make ex1' si c'est le 2 faites 'make ex2'"

ex1:
	gcc -o main src/exercice1/*.c -ggdb
	./main

ex2:
	gcc -o main src/exercice2/*.c -ggdb
	./main
compil:
	gcc -o main src/exercice1/*.c -ggdb -Wall
run:
	./main
