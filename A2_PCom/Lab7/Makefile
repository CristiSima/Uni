# Protocoale de comunicatii
# Laborator 7 - TCP
# Echo Server
# Makefile

CFLAGS = -Wall -g

# Portul pe care asculta serverul
PORT = 1337 

# Adresa IP a serverului
#IP_SERVER = 127.0.0.127
IP_SERVER = 192.168.187.196
all: server client 

# Compileaza server.c
server: server.c

# Compileaza client.c
client: client.c

.PHONY: clean run_server run_client

# Ruleaza serverul
run_server:
	./server ${IP_SERVER} ${PORT}

# Ruleaza clientul 	
run_client:
	./client ${IP_SERVER} ${PORT}

clean:
	rm -f server client
