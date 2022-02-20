#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"
#include "utils.h"

linked_list_t*
ll_create(unsigned int data_size)
{
	linked_list_t* list=malloc(sizeof(linked_list_t));
	DIE(list==NULL,"malloc linked_list_t failed");

	list->data_size=data_size;
	list->size=0;
	list->head=NULL;
	return list;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Daca n >= nr_noduri, noul nod se adauga la finalul listei. Daca
 * n < 0, eroare.
 */
void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
	ll_node_t *node,*aux;
	unsigned int i;
	DIE(n<0,"wrong place");
	DIE(list==NULL,"got no list");

	node=malloc(sizeof(ll_node_t));
	DIE(node==NULL,"malloc ll_node_t failed");

	node->data=malloc(list->data_size);
	DIE(node->data==NULL,"malloc node->data failed");

	memcpy(node->data,new_data,list->data_size);

	if(n==0 || list->size==0)
	{
		node->next=list->head;
		list->head=node;
	}
	else
	{
		i=1;
		for(aux=list->head;aux->next!=NULL && i<n;i++)	aux=aux->next;
		node->next=aux->next;
		aux->next=node;
	}
	list->size++;
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se elimina nodul de
 * la finalul listei. Daca n < 0, eroare. Functia intoarce un pointer spre acest
 * nod proaspat eliminat din lista. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */
ll_node_t*
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
	ll_node_t *node,*aux;
	unsigned int i;
	DIE(n<0,"wrong place");
	DIE(list==NULL,"got no list");
	DIE(list->head==NULL,"list is empty");

	if(n==0)
	{
		node=list->head;
		list->head=list->head->next;
	}
	else
	{
		i=1;
		for(aux=list->head;aux->next->next!=NULL && i<n;i++)	aux=aux->next;
		node=aux->next;
		aux->next=aux->next->next;
	}
	node->next=NULL;
	list->size--;
	return node;
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int
ll_get_size(linked_list_t* list)
{
	ll_node_t *aux;
	int size=0;
	DIE(list==NULL,"got no list");

	for(aux=list->head;aux!=NULL;aux=aux->next) size++;
	return size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista si actualizeaza la
 * NULL valoarea pointerului la care pointeaza argumentul (argumentul este un
 * pointer la un pointer).
 */
void
ll_free(linked_list_t** pp_list)
{
	ll_node_t *aux;
	DIE(pp_list[0]==NULL,"got no list");

	while(pp_list[0]->head!=NULL)
	{
		aux=pp_list[0]->head;
		pp_list[0]->head=pp_list[0]->head->next;
		free(aux->data);
		free(aux);
	}
	free(pp_list[0]);
	pp_list[0]=NULL;
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista inlantuita separate printr-un spatiu.
 */
void
ll_print_int(linked_list_t* list)
{
	ll_node_t *aux;
	DIE(list==NULL,"got no list");

	for(aux=list->head;aux!=NULL;aux=aux->next)
		printf("%d ",((int*)(aux->data))[0]);
    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista inlantuita, separate printr-un spatiu.
 */
void
ll_print_string(linked_list_t* list)
{
	ll_node_t *aux;
	DIE(list==NULL,"got no list");
	
	for(aux=list->head;aux!=NULL;aux=aux->next)
		printf("%s ",(char*)(aux->data));
    printf("\n");
}
