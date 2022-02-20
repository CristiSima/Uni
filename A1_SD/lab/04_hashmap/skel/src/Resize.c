#include "Hashtable.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_STRING_SIZE		256

typedef unsigned int uint;

struct InfoPrieten {
	char nume[MAX_STRING_SIZE];
	char facultate[MAX_STRING_SIZE];
	char mancare_preferata[MAX_STRING_SIZE];
	int varsta;
};

static void
print_prieten(struct InfoPrieten *p_info)
{
	if (p_info == NULL) {
		printf("Prieten inexistent!\n");
		return;
	}
	printf("* Nume: %s | ", p_info->nume);
	printf("Varsta: %d | ", p_info->varsta);
	printf("Facultate: %s | ", p_info->facultate);
	printf("Mancare preferata: %s\n", p_info->mancare_preferata);
}


hashtable_t *ht_resize(hashtable_t *ht,unsigned int new_hmax)
{
	hashtable_t *new_ht=ht_create(new_hmax,ht->hash_function,ht->compare_function);
	linked_list_t *list;
	struct info *info;
	struct info *new_info;

	for(uint i=0;i<ht->hmax;i++)
	{
		for(ll_node_t *node=ht->buckets[i]->head;node!=NULL;node=node->next)
		{
			info=node->data;
			// key_size & value_size needed
			list=new_ht->buckets[ht->hash_function(info->key)%new_hmax];
			ll_add_nth_node(list,0,info);
		}
	}
	return new_ht;
}

void ht_resize_replace(hashtable_t *ht,unsigned int new_hmax)
{
	linked_list_t **new_buckets=malloc(sizeof(linked_list_t*)*new_hmax);
	for(unsigned int i=0;i<new_hmax;i++)
		new_buckets[i]=ll_create(sizeof(struct info));//?

	linked_list_t *list;
	struct info *info;

	for(uint i=0;i<ht->hmax;i++)
	{
		while(ht->buckets[i]->head)
		{
			info=ht->buckets[i]->head->data;
			list=new_buckets[ht->hash_function(info->key)%new_hmax];
			ll_add_nth_node(list,0,info);
			free(info);
			free(ll_remove_nth_node(ht->buckets[i],0));
		}
		free(ht->buckets[i]);
	}
	free(ht->buckets);
	ht->buckets=new_buckets;
	ht->hmax=new_hmax;
}

int main()
{
	struct InfoPrieten informatii_prieteni[4];
	struct InfoPrieten *p_infoPrieten;
	int current_key;

	/* Initializare structuri informatii prieteni. */
	strcpy(informatii_prieteni[0].nume, "Alexandra");
	informatii_prieteni[0].varsta = 20;
	strcpy(informatii_prieteni[0].facultate, "ACS");
	strcpy(informatii_prieteni[0].mancare_preferata, "tortellini");

	strcpy(informatii_prieteni[1].nume, "Alin");
	informatii_prieteni[1].varsta = 21;
	strcpy(informatii_prieteni[1].facultate, "ACS");
	strcpy(informatii_prieteni[1].mancare_preferata, "noodles");

	strcpy(informatii_prieteni[2].nume, "Alexandru");
	informatii_prieteni[2].varsta = 22;
	strcpy(informatii_prieteni[2].facultate, "ACS");
	strcpy(informatii_prieteni[2].mancare_preferata, "pizza");

	strcpy(informatii_prieteni[3].nume, "Loredana");
	informatii_prieteni[3].varsta = 21;
	strcpy(informatii_prieteni[3].facultate, "ACS");
	strcpy(informatii_prieteni[3].mancare_preferata, "pizza");

	hashtable_t *old=ht_create(10, hash_function_string,
				compare_function_strings);
	hashtable_t *new;

	ht_put(old,"Alin", MAX_STRING_SIZE, &informatii_prieteni[1],sizeof(struct InfoPrieten));
	ht_put(old,"Loredana",MAX_STRING_SIZE,&informatii_prieteni[3], sizeof(struct InfoPrieten));

	print_prieten(ht_get(old, "Alin"));

	// resize would need value_size & key_size
	// new=ht_resize(old,2);
	// print_prieten(ht_get(new, "Alin"));
	// ht_free(new);

	ht_resize_replace(old,20);
	print_prieten(ht_get(old, "Alin"));
	ht_free(old);
	return 0;
}
// gcc Resize.c Hashtable.c LinkedList.c -o Resize -g;./Resize
