#include "Banca.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

client_t *client_create(char *name,unsigned int pin,int balance)
{
	client_t *client=malloc(sizeof(client_t));
	strncpy(client->name,name,CLIENT_NAME_LEN-1);
	client->pin=pin;
	client->transactions=ll_create(sizeof(transaction_t));
	client->balance=balance;

	return client;
}

void client_free(client_t *client)
{
	ll_free(&client->transactions);
	free(client);
}

transaction_t *transaction_create(char *place, ushort day, ushort month,
									ushort year, int ammount)
{
	transaction_t *transaction=malloc(sizeof(transaction_t));
	strncpy(transaction->place,place,TRANSACTION_PLACE_LEN-1);
	transaction->day=day;
	transaction->month=month;
	transaction->year=year;
	transaction->ammount=ammount;

	return transaction;
}

void transaction_print(transaction_t *transaction)
{
	printf("\t- %d on %02d.%02d.%04d\n",transaction->ammount, transaction->day,
 									transaction->month, transaction->year);
}

void transaction_write_date(transaction_t *transaction, char *date)
{
	sprintf(date,"%02d.%02d.%04d",transaction->day,
										transaction->month, transaction->year);
}

void client_show_transactions(client_t *client)
{
	printf("%s's transactions:\n",client->name);
	for(ll_node_t *node=client->transactions->head;node!=NULL;node=node->next)
	{
		transaction_print(node->data);
	}
	printf("\n");
}

void ht_free_Wfree(hashtable_t *ht, free_function_t value_free)
{
	unsigned int i;
	linked_list_t *list;

	for(i=0;i<ht->hmax;i++)
	{
		list=ht->buckets[i];

		while(list->size)
		{
			free(((struct info*)(list->head->data))->key);
			value_free(((struct info*)(list->head->data))->value);
			free(list->head->data);
			free(ll_remove_nth_node(list,0));
		}
		free(list);
	}
	free(ht->buckets);
	free(ht);
}

void ll_free_wrapper(void *list)
{
	ll_free(((linked_list_t**)(&list)));
}

void add_to_ht_list(hashtable_t *ht,char *key, uint key_size, void *value, uint value_size)
{
	linked_list_t *list;
	if(!ht_has_key(ht,key))
	{
		list=ll_create(value_size);
		ht_put(ht,key,key_size,list,sizeof(linked_list_t));
		free(list);
	}
	list=ht_get(ht,key);
	ll_add_nth_node(list,list->size,value);
}

void get_transactions_on(hashtable_t *ht, char *date)
{
	linked_list_t *list=ht_get(ht,date);
	if(list)
	{
		printf("Transactions on %s\n",date);
		for(ll_node_t *node=list->head;node!=NULL;node=node->next)
			transaction_print(node->data);
	}
	else
		printf("No transactions on %s\n",date);
	printf("\n");
}

int main()
{
	client_t **clients=malloc(sizeof(client_t*)*5);
	transaction_t **transactions=malloc(sizeof(transaction_t*)*10);
	hashtable_t *name_ht=ht_create(20,hash_function_string,compare_function_strings);
	hashtable_t *transaction_ht=ht_create(50,hash_function_string,compare_function_strings);
	clients[0]=client_create("Rick",993,420);
	clients[1]=client_create("Morty",96,96);
	clients[2]=client_create("Bet",65,49);
	clients[3]=client_create("Mr",563,36);
	clients[4]=client_create("MR",84,3);
	transactions[0]=transaction_create("a",1 ,2,2000,61);
	transactions[1]=transaction_create("b",1 ,2,1995,66);
	transactions[2]=transaction_create("c",10,6,2009,36);
	transactions[3]=transaction_create("d",1 ,2,2014,76);
	transactions[4]=transaction_create("e",1 ,2,2006,36);
	transactions[5]=transaction_create("f",1 ,2,2006,76);
	transactions[6]=transaction_create("g",1 ,2,2011,66);
	transactions[7]=transaction_create("h",10,7,2009,56);
	transactions[8]=transaction_create("i",1 ,2,2017,60);
	transactions[9]=transaction_create("j",1 ,2,2019,600);
	char date[15];
	linked_list_t *list;
	for(int i=0;i<5;i++)
	{
		ht_put(name_ht,clients[i]->name,CLIENT_NAME_LEN,clients[i],sizeof(client_t));

		ll_add_nth_node(clients[i]->transactions,0,transactions[i*2]);
		transaction_write_date(transactions[i*2],date);
		add_to_ht_list(transaction_ht,date,15,transactions[i*2],sizeof(transaction_t));

		ll_add_nth_node(clients[i]->transactions,1,transactions[i*2+1]);
		transaction_write_date(transactions[i*2+1],date);
		add_to_ht_list(transaction_ht,date,15,transactions[i*2+1],sizeof(transaction_t));
	}

	// b
	client_show_transactions(clients[2]);

	// c
	for(int i=0;i<5;i++)
	{
		if(clients[i]->balance<50)
			ht_remove_entry(name_ht,clients[i]->name);
	}

	// d
	get_transactions_on(transaction_ht,"01.02.2006");
	get_transactions_on(transaction_ht,"01.02.2106");


	// FREE
	ht_free(name_ht);
	ht_free_Wfree(transaction_ht,ll_free_wrapper);

	for(int i=0;i<10;i++)
		free(transactions[i]);
	free(transactions);

	for(int i=0;i<5;i++)
		client_free(clients[i]);
	free(clients);

	return 0;
}
// gcc Banca.c Hashtable.c LinkedList.c -o Banca
