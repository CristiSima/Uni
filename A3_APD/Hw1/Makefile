CC=gcc
CFLAGS= -Wall -Wextra -lm
LINKS= -lm -lpthread

build:	tema1

linked_list.o: linked_list.c
	$(CC) $(CFLAGS) -c linked_list.c -o linked_list.o $(LINKS)
tema1: main.c linked_list.o
	$(CC) $(CFLAGS) main.c linked_list.o -o tema1 $(LINKS)


clean:
	rm -rf tema1 linked_list.o
