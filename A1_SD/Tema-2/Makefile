CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -g -O0
LOAD=load_balancer
SERVER=server
LIBS=hashtable linked_list

.PHONY: build clean

build: tema2

tema2: main.o $(LOAD).o $(SERVER).o hashtable.o linked_list.o
	$(CC) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) $^ -c

hashtable.o: hashtable.c hashtable.h
	$(CC) $(CFLAGS) $^ -c

linked_list.o: linked_list.c linked_list.h
	$(CC) $(CFLAGS) $^ -c

$(SERVER).o: $(SERVER).c $(SERVER).h
	$(CC) $(CFLAGS) $^ -c

$(LOAD).o: $(LOAD).c $(LOAD).h
	$(CC) $(CFLAGS) $^ -c

clean:
	rm -f *.o tema2 *.h.gch

pack:
	zip -FSr 314CA_SimacheCristian_Tema2.zip README.md Makefile *.c *.h
