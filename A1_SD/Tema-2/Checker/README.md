# LinkedList.c

## ll_create
```c
linked_list_t* ll_create(unsigned int data_size)
```
allocates memory and initializes

## ll_add_nth_node
```c
void ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
```
creates a new node(copying the data in it)<BR>
if n or the list's size are 0 that node becomes the new head<BR>
otherwise the last node or the one before the nth is selected and the new node is inserted afterwards

## ll_remove_nth_node
```c
ll_node_t* ll_remove_nth_node(linked_list_t* list, unsigned int n)
```
allocates memory and initializes<BR>
if n is 0 the head becomes the next node<BR>
otherwise the one before the nth is selected and the nth is skiped<BR>
the function return a pointer to the removed node with the next field set to NULL

## ll_get_size
```c
unsigned int ll_get_size(linked_list_t* list)
```
iterates over the list to calculate the number of elements,just in case

## ll_free
```c
void ll_free(linked_list_t** pp_list)
```
iterates over the list to free the data in the nodes,and the nodes<BR>
then frees the list and sets the pointer to the freed list to NULL

## ll_print_int/ll_print_string
```c
void ll_print_int(linked_list_t* list)
void ll_print_string(linked_list_t* list)
```
iterates over the list and prints using ``%d`` and ``%s``

<BR>
<BR>

# Hashtable.c

## ht_create
```c
hashtable_t *ht_create(unsigned int hmax, unsigned int (*hash_function)(void*), int (*compare_function)(void*, void*));
```
allocates memory

## ht_put
```c
void ht_put(hashtable_t *ht, void *key, unsigned int key_size, void *value, unsigned int value_size)
```
selects the apropriate list and searches it for the key<BR>
if the key is found the memory is copied over value<BR>
else a new info object is created and appended to the list

## ht_get
```c
void *ht_get(hashtable_t *ht, void *key)
```
selects the apropriate list and searches it for the key

## ht_has_key
```c
int ht_has_key(hashtable_t *ht, void *key)
```
selects the apropriate list and searches it for the key

## ht_remove_entry
```c
void ht_remove_entry(hashtable_t *ht, void *key)
```
selects the apropriate list and searches it for the key

## ht_free
```c
void ht_free(hashtable_t *ht)
```
iterates over all the buckets and frees the list

<BR>
<BR>

# server.c

## init_server_memory
```c
server_memory* init_server_memory()
```
allocates memory

## server_store
```c
void server_store(server_memory* server, char* key, char* value)
```
wrapper for `ht_put`

## server_remove
```c
void server_remove(server_memory* server, char* key)
```
wrapper for `ht_remove_entry`

## server_retrieve
```c
char* server_retrieve(server_memory* server, char* key)
```
wrapper for `ht_get`

## free_server_memory
```c
void free_server_memory(server_memory* server)
```
wrapper for `ht_free`

<BR>
<BR>

# load_balancer.c

## load_balancer
```c
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
```
`->servers` stores servers at the same index as their id  <BR>
`->map` stores a list of servers with their hash, for each clone, sorted by `->hash`

## init_load_balancer
```c
load_balancer* init_load_balancer()
```
allocates memory and initializes values<BR>
`->servers[X]` is NULL for servers that don't exist

## get_server_for
```c
int get_server_for(load_balancer *main, char *key)
```
iterates over `->map` and returns the id of the server where that key should go

## loader_store
```c
void loader_store(load_balancer* main, char* key, char* value, int* server_id)
```
wrapper for `server_store`

## loader_retrieve
```c
char* loader_retrieve(load_balancer* main, char* key, int* server_id)
```
wrapper for `server_retrieve`

## __loader_add_migrate
```c
static void __loader_add_migrate(load_balancer *main, int server_id)
```
iterates over all the lists for all the buckets and reassigns values

## loader_add_server
```c
void loader_add_server(load_balancer* main, int server_id)
```
allocates the server<BR>
adds the entries for that server to `->map`<BR>
calls `__loader_add_migrate` on all the servers that appear before it in `->map`

## loader_remove_server
```c
void loader_remove_server(load_balancer* main, int server_id)
```
removes the entries for the server from `->map`<BR>
reassigns all the values stored on that server<BR>
then frees the server and resets `->servers[server_id]`

## free_load_balancer
```c
void free_load_balancer(load_balancer* main)
```
frees all the servers
