CC=gcc
CFLAGS=-Wall -Wextra -std=c99

build:
	$(CC) $(CFLAGS) *.c -o main

pack:
	zip -FSr 314CA_SimacheCristian_Tema1.zip README Makefile *.c *.h

clean:
	rm -f main

.PHONY: pack clean
