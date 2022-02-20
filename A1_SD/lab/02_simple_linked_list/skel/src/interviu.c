#include "interviu.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void ll_remove_duplicates(linked_list_t *list)
{
	ll_node_t *aux,*node;
	DIE(list==NULL,"got no list");

	for(aux=list->head;aux!=NULL && aux->next!=NULL;aux=aux->next)
	{
		while(aux->next!=NULL && !memcmp(aux->data,aux->next->data,list->data_size))
		{
			node=aux->next;
			aux->next=aux->next->next;
			list->size--;

			free(node->data);
			free(node);
		}
	}
}

void ll_make_circular(linked_list_t *list)
{
	ll_node_t *aux;
	DIE(list==NULL,"got no list");

	for(aux=list->head;aux->next!=NULL;aux=aux->next);
	aux->next=list->head;
}

int ll_is_circular(linked_list_t *list)
{
	ll_node_t *aux;
	unsigned int i=1;
	DIE(list==NULL,"got no list");

	for(aux=list->head;i<list->size;i++) aux=aux->next;
	return aux->next==list->head;
}

void ll_unmake_circular(linked_list_t *list)
{
	ll_node_t *aux;
	unsigned int i=1;
	DIE(list==NULL,"got no list");

	for(aux=list->head;i<list->size;i++) aux=aux->next;
	aux->next=NULL;
}

void ll_make_cicle(linked_list_t *list,int n)
{
	ll_node_t *aux,*node;
	unsigned int i=1;
	DIE(list==NULL,"got no list");

	n++;
	if(list->size<n)
		n=list->size;

	for(aux=list->head;i<list->size;i++)
	{
		if(i==n)
			node=aux;
		aux=aux->next;
	}
	aux->next=node;
}

int ll_has_cicle(linked_list_t *list)
{
	ll_node_t *aux;
	unsigned int i=1;
	DIE(list==NULL,"got no list");

	for(aux=list->head;i<list->size;i++) aux=aux->next;
	return aux->next!=NULL;
}

void ll_unmake_cicle(linked_list_t *list)
{
	ll_node_t *aux;
	unsigned int i=1;
	DIE(list==NULL,"got no list");

	for(aux=list->head;i<list->size;i++) aux=aux->next;
	aux->next=NULL;
}

ll_node_t *ll_get_nth_node(linked_list_t *list,int n)
{
	ll_node_t *aux;
	unsigned int i=1;
	DIE(list==NULL,"got no list");

	if(list->size<=n)
		n=list->size;
		
	for(aux=list->head;i<n;i++) aux=aux->next;
	return aux;
}

ll_node_t *ll_get_last_nth_node(linked_list_t *list,int n)
{
	ll_node_t *aux;
	unsigned int i=1;
	DIE(list==NULL,"got no list");

	if(list->size<=n)
		n=list->size;
	n=list->size-n;

	for(aux=list->head;i<n;i++) aux=aux->next;
	return aux;
}

void test_dublicats()
{
	linked_list_t *list=ll_create(sizeof(int));
	int nr;
	nr=7;ll_add_nth_node(list,10,&nr);
	nr=3;ll_add_nth_node(list,10,&nr);
	nr=3;ll_add_nth_node(list,10,&nr);
	nr=2;ll_add_nth_node(list,10,&nr);
	nr=2;ll_add_nth_node(list,10,&nr);
	nr=2;ll_add_nth_node(list,10,&nr);
	nr=2;ll_add_nth_node(list,10,&nr);
	// nr=1;ll_add_nth_node(list,10,&nr);

	ll_print_int(list);
	ll_remove_duplicates(list);
	ll_print_int(list);

}

void test_circular()
{
	linked_list_t *list=ll_create(sizeof(int));
	int nr;
	nr=7;ll_add_nth_node(list,10,&nr);
	nr=3;ll_add_nth_node(list,10,&nr);
	nr=3;ll_add_nth_node(list,10,&nr);
	nr=2;ll_add_nth_node(list,10,&nr);
	nr=2;ll_add_nth_node(list,10,&nr);
	nr=2;ll_add_nth_node(list,10,&nr);
	nr=2;ll_add_nth_node(list,10,&nr);

	ll_print_int(list);
	printf("list.is_circular: %d\n",ll_is_circular(list));
	ll_make_circular(list);
	printf("list.is_circular: %d\n",ll_is_circular(list));
	ll_unmake_circular(list);
	printf("list.is_circular: %d\n",ll_is_circular(list));
	ll_print_int(list);

}

void test_cicle()
{
	linked_list_t *list=ll_create(sizeof(int));
	int nr;
	nr=1;ll_add_nth_node(list,10,&nr);
	nr=2;ll_add_nth_node(list,10,&nr);
	nr=3;ll_add_nth_node(list,10,&nr);
	nr=4;ll_add_nth_node(list,10,&nr);
	nr=5;ll_add_nth_node(list,10,&nr);
	nr=6;ll_add_nth_node(list,10,&nr);
	nr=7;ll_add_nth_node(list,10,&nr);

	ll_print_int(list);
	printf("list.has_cicle: %d\n",ll_has_cicle(list));
	ll_make_cicle(list,10);
	printf("list.has_cicle: %d\n",ll_has_cicle(list));
	ll_unmake_cicle(list);
	printf("list.has_cicle: %d\n",ll_has_cicle(list));
	ll_print_int(list);

}

void test_get()
{
	linked_list_t *list=ll_create(sizeof(int));
	int nr;
	nr=1;ll_add_nth_node(list,10,&nr);
	nr=2;ll_add_nth_node(list,10,&nr);
	nr=3;ll_add_nth_node(list,10,&nr);
	nr=4;ll_add_nth_node(list,10,&nr);
	nr=5;ll_add_nth_node(list,10,&nr);
	nr=6;ll_add_nth_node(list,10,&nr);
	nr=7;ll_add_nth_node(list,10,&nr);

	ll_print_int(list);
	((int*)(ll_get_nth_node(list,0)->data))[0]=9;
	ll_print_int(list);
	((int*)(ll_get_nth_node(list,10)->data))[0]=8;
	ll_print_int(list);
	((int*)(ll_get_last_nth_node(list,0)->data))[0]=9;
	ll_print_int(list);
}

int main()
{
	// test_dublicats();
	// test_circular();
	// test_cicle();
	// test_get();
	return 0;
}
// gcc interviu.c LinkedList.c -o interviu;./interviu;rm interviu
