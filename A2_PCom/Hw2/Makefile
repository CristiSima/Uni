CC=gcc
CARGS=-Wall -Wextra
LIBS=trie_dic.o linked_list.o

PORT = 1337

IP_SERVER = 127.127.127.127

build: server subscriber

trie_dic.o:trie_dic.c
	$(CC) $(CARGS) -c trie_dic.c -o trie_dic.o

linked_list.o:linked_list.c
	$(CC) $(CARGS) -c linked_list.c -o linked_list.o

server:server.c $(LIBS)
	$(CC) $(CARGS) $(LIBS) server.c -o server

subscriber:subscriber.c $(LIBS)
	$(CC) $(CARGS) $(LIBS) subscriber.c -o subscriber

run_server: server
	./server ${PORT}

run_subscriber: subscriber
	./subscriber $$PPID ${IP_SERVER} ${PORT}

echo:
	echo $$PPID

clean:
	rm server subscriber $(LIBS)

pack:
	zip -r 324CA_Simache_Cristian_Ioan_Tema2.zip *.c *.h readme.txt Makefile
