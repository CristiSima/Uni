CC=gcc -g
CFLAGS=-I.

client: client.c requests.c helpers.c buffer.c
	$(CC) -o client client.c requests.c helpers.c parson.c buffer.c -Wall

run: client
	./client

clean:
	rm -f *.o client

pack:
	zip Simache_Cristian_324CA_Tema3PC.zip *.c *.h README
