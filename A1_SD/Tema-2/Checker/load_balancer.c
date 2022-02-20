// Copyright 2021 Simache Cristian Ioan
#include <stdlib.h>
#include <string.h>

#include "load_balancer.h"
#include "utils.h"

typedef struct server_hash_id server_hash_id;
struct server_hash_id
{
	unsigned int hash;
	int id;
};

struct load_balancer {
	int server_count;
	server_memory *servers[MAX_SERVERS];
	server_hash_id map[SERVER_REFERENCES*MAX_SERVERS];
};

unsigned int hash_function_servers(void *a)
{
	unsigned int uint_a = *((unsigned int *)a);

	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = (uint_a >> 16u) ^ uint_a;
	return uint_a;
}

unsigned int hash_function_key(void *a)
{
	unsigned char *puchar_a = (unsigned char *) a;
	unsigned int hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c;

	return hash;
}

load_balancer* init_load_balancer()
{
	load_balancer *balancer = malloc(sizeof(load_balancer));

	for(int i = 0; i < MAX_SERVERS; i++)
		balancer-> servers[i] = NULL;
	balancer-> server_count = 0;

	return balancer;
}

int get_server_for(load_balancer *main, char *key)
{
	int end = SERVER_REFERENCES*main-> server_count;
	unsigned int hash = hash_function_key(key);

	for(int i = 0; i < end; i++)
		if (hash <= main-> map[i].hash)
			return main-> map[i].id;

	return main-> map[0].id;
}

void loader_store(load_balancer* main, char* key, char* value, int* server_id)
{
	server_id[0] = get_server_for(main, key);
	server_store(main-> servers[server_id[0]], key, value);
}


char* loader_retrieve(load_balancer* main, char* key, int* server_id)
{
	server_id[0] = get_server_for(main, key);

	return server_retrieve(main-> servers[server_id[0]], key);
}
static void __loader_add_migrate(load_balancer *main, int server_id)
{
	unsigned int hmax = main-> servers[server_id]-> memory-> hmax;
	int where;
	hashtable_t *ht = main-> servers[server_id]-> memory;
	struct info *pair;
	// a value is removed only after aux moved to the next node to not
	// remove the curent node
	void *del_key = NULL;

	for(unsigned int j = 0; j < hmax; j++)
	{
		for(ll_node_t *aux = ht-> buckets[j]-> head; aux!= NULL; aux = aux-> next)
		{
			if (del_key!= NULL)
				ht_remove_entry(main-> servers[server_id]-> memory, del_key);

			del_key = NULL;
			pair = aux-> data;

			if (get_server_for(main, pair-> key)!= server_id)
			{
				del_key = pair-> key;
				loader_store(main, pair-> key, pair-> value, &where);
			}
		}
	}

	if (del_key!= NULL)
		ht_remove_entry(main-> servers[server_id]-> memory, del_key);
}
void loader_add_server(load_balancer* main, int server_id)
{
	unsigned int hash, id;
	int added = 0;
	int end = SERVER_REFERENCES*main-> server_count;

	main-> servers[server_id] = init_server_memory();
	main-> server_count++;

	// inserts in map
	for(int i = 0; i < SERVER_REFERENCES; i++)
	{
		added = 0;
		id = i*REFERENCE_OFFSET+server_id;
		hash = hash_function_servers(&id);

		for(int j = 0; j < end; j++)
			if (hash < main-> map[j].hash)
			{
				for(int k = end; k > j; k--)
					main-> map[k] = main-> map[k-1];

				main-> map[j].hash = hash;
				main-> map[j].id = server_id;

				end++;
				added = 1;
				break;
			}

		if (!added)
		{
			main-> map[end].hash = hash;
			main-> map[end].id = server_id;

			end++;
		}
	}

	if (main-> map[end-1].id == server_id )
		__loader_add_migrate(main, main-> map[0].id);

	for(int i = 0; i < end-1; i++)
		if (main-> map[i].id == server_id)
			__loader_add_migrate(main, main-> map[i+1].id);
}


void loader_remove_server(load_balancer* main, int server_id)
{
	int end = SERVER_REFERENCES*main-> server_count;
	int i = 0;

	// removes from map
	while(i < end)
		if (main-> map[i].id == server_id)
		{
			end--;
			for(int j = i; j < end; j++)
				main-> map[j] = main-> map[j+1];
		}
		else
			i++;

	main-> server_count--;

	struct info *pair;
	int target_id;
	unsigned int hmax = main-> servers[server_id]-> memory-> hmax;
	hashtable_t *ht = main-> servers[server_id]-> memory;

	// reassigns values
	for(unsigned int i = 0; i < hmax; i++)
		for(ll_node_t *aux = ht-> buckets[i]-> head; aux!= NULL; aux = aux-> next)
		{
			pair = aux-> data;
			loader_store(main, pair-> key, pair-> value, &target_id);
		}

	free_server_memory(main-> servers[server_id]);
	main-> servers[server_id] = NULL;
}

void free_load_balancer(load_balancer* main)
{
	for(int i = 0; i < MAX_SERVERS; i++)
		if (main-> servers[i])
			free_server_memory(main-> servers[i]);

	free(main);
}
