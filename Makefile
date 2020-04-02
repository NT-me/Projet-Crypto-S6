rc:
	gcc -o main src/*.c -ggdb
	./main

compil:
	gcc -o main src/*.c -ggdb -Wall
run:
	./main
