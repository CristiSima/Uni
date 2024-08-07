// Copyright 2021 Simache Cristian Ioan
#include <stdlib.h>
#include <string.h>

#include "server.h"

server_memory* init_server_memory()
{
	server_memory *server = malloc(sizeof(server_memory));
	server-> memory = ht_create(100, hash_function_key, compare_function_strings);

	return server;
}

void server_store(server_memory* server, char* key, char* value)
{
	ht_put(server-> memory, key, strlen(key)+1, value, strlen(value)+1);
}

void server_remove(server_memory* server, char* key)
{
	ht_remove_entry(server-> memory, key);
}

char* server_retrieve(server_memory* server, char* key)
{
	return ht_get(server-> memory, key);
}

void free_server_memory(server_memory* server)
{
	ht_free(server-> memory);
	free(server);
}
