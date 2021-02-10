CC=gcc
CFLAGS= -Wall
OBJ=E2_h.o E2.o

build: E2_h E2
	$(CC) $(OBJ) -o E2

E2_h: E2_h.c
	$(CC) $(CFLAGS) -c $@.c

E2: E2.c
	$(CC) $(CFLAGS) -c $@.c

run: build
	./E2 2 3

clean: 
	rm E2.o E2_h.o
