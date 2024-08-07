// Copyright 2021 Simache Cristian-Ioan & Alexandru-Cosmin Mihai

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#include "hashtable.h"

#define MAX_BUCKET_SIZE 64

int
compare_function_ints(void *a, void *b)
{
	int int_a = *((int *)a);
	int int_b = *((int *)b);

	if (int_a == int_b) {
		return 0;
	} else if (int_a < int_b) {
		return -1;
	} else {
		return 1;
	}
}

int
compare_function_strings(void *a, void *b)
{
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	return strcmp(str_a, str_b);
}

unsigned int
hash_function_int(void *a)
{
	/*
	 * Credits: https://stackoverflow.com/a/12996028/7883884
	 */
	unsigned int uint_a = *((unsigned int *)a);

	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = (uint_a >> 16u) ^ uint_a;
	return uint_a;
}

unsigned int
hash_function_string(void *a)
{
	/*
	 * Credits: http://www.cse.yorku.ca/~oz/hash.html
	 */
	unsigned char *puchar_a = (unsigned char*) a;
	unsigned long hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

	return hash;
}

/*
 * Functie apelata dupa alocarea unui hashtable pentru a-l initializa.
 * Trebuie alocate si initializate si listele inlantuite.
 */
hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*))
{
	hashtable_t *ht=malloc(sizeof(hashtable_t));

	ht->hash_function=hash_function;
	ht->compare_function=compare_function;
	ht->size=0;
	ht->hmax=hmax;

	ht->buckets=malloc(sizeof(linked_list_t*)*hmax);
	for(unsigned int i=0;i<hmax;i++)
		ht->buckets[i]=ll_create(sizeof(struct info));

	return ht;
}

void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size)
{
	if(ht==NULL || key==NULL || value==NULL)
		return;

	linked_list_t *list=ht->buckets[ht->hash_function(key)%ht->hmax];
	for(ll_node_t *node=list->head;node!=NULL;node=node->next)
		if(!ht->compare_function(key,((struct info*)(node->data))->key))
		{
			memcpy(((struct info*)(node->data))->value,value,value_size);
			return;
		}

	ht->size++;
	struct info *info=malloc(sizeof(struct info));
	info->key=malloc(key_size);
	info->value=malloc(value_size);

	memcpy(info->key,key,key_size);
	memcpy(info->value,value,value_size);

	ll_add_nth_node(list,0,info);
	free(info);
}

void *
ht_get(hashtable_t *ht, void *key)
{
	if(ht==NULL || key==NULL)
		return NULL;

	linked_list_t *list=ht->buckets[ht->hash_function(key)%ht->hmax];

	for(ll_node_t *node=list->head;node!=NULL;node=node->next)
		if(!ht->compare_function(key,((struct info*)(node->data))->key))
			return ((struct info*)(node->data))->value;

	return NULL;
}

int
ht_has_key(hashtable_t *ht, void *key)
{
	if(ht==NULL || key==NULL)
		return 0;

	linked_list_t *list=ht->buckets[ht->hash_function(key)%ht->hmax];

	for(ll_node_t *node=list->head;node!=NULL;node=node->next)
		if(!ht->compare_function(key,((struct info*)(node->data))->key))
			return 1;

	return 0;
}

void
ht_remove_entry(hashtable_t *ht, void *key)
{
	if(ht==NULL || key==NULL)
		return;
	linked_list_t *list=ht->buckets[ht->hash_function(key)%ht->hmax];
	ll_node_t *node,*aux;

	ht->size--;
	list->size--;

	if(!ht->compare_function(key,((struct info*)(list->head->data))->key))
	{
		node=list->head;
		list->head=list->head->next;

		free(((struct info*)(node->data))->value);
		free(((struct info*)(node->data))->key);
		free(node->data);
		free(node);

		return;
	}
	for(aux=list->head;aux!=NULL ;aux=aux->next)
		if(!ht->compare_function(key,((struct info*)(aux->next->data))->key))
		{
			node=aux->next;
			aux->next=aux->next->next;

			free(((struct info*)(node->data))->value);
			free(((struct info*)(node->data))->key);
			free(node->data);
			free(node);

			return;
		}
}

void
ht_free(hashtable_t *ht)
{
	/* TODO */
	unsigned int i;
	linked_list_t *list;

	for(i=0;i<ht->hmax;i++)
	{
		list=ht->buckets[i];

		while(list->size)
		{
			free(((struct info*)(list->head->data))->key);
			free(((struct info*)(list->head->data))->value);
			free(list->head->data);
			free(ll_remove_nth_node(list,0));
		}
		free(list);
	}

	free(ht->buckets);
	free(ht);
}

unsigned int
ht_get_size(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->size;
}

unsigned int
ht_get_hmax(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->hmax;
}
