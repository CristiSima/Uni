build:
	mpicc -g -o tema3 tema3.c -Wall -lm

run: build
	mpirun --oversubscribe -np 12 ./tema3 12 0

clean:
	rm -rf tema3

pack:
	 zip Tema3.zip *.c Makefile README
