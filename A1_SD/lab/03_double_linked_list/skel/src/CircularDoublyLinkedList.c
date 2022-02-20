#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CircularDoublyLinkedList.h"
#include "utils.h"

/*
 * Functie care trebuie apelata pentru alocarea si initializarea unei liste.
 * (Setare valori initiale pentru campurile specifice structurii LinkedList).
 */
doubly_linked_list_t*
dll_create(unsigned int data_size)
{
	doubly_linked_list_t* list=malloc(sizeof(doubly_linked_list_t));
	DIE(list==NULL,"malloc doubly_linked_list_t failed");

	list->data_size=data_size;
	list->size=0;
	list->head=NULL;
	return list;
}

/*
 * Functia intoarce un pointer la nodul de pe pozitia n din lista.
 * Pozitiile din lista sunt indexate incepand cu 0 (i.e. primul nod din lista se
 * afla pe pozitia n=0). Daca n >= nr_noduri, atunci se intoarce nodul de pe
 * pozitia rezultata daca am "cicla" (posibil de mai multe ori) pe lista si am
 * trece de la ultimul nod, inapoi la primul si am continua de acolo. Cum putem
 * afla pozitia dorita fara sa simulam intreaga parcurgere? Daca n < 0, eroare.
 */
dll_node_t*
dll_get_nth_node(doubly_linked_list_t* list, unsigned int n)
{
	dll_node_t *node;
	unsigned int i;
	DIE(n<0,"wrong place");
	DIE(list==NULL,"got no list");
	DIE(list->head==NULL,"list is empty");

	i=0;
	for(node=list->head;i<n;i++)	node=node->next;
	return node;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Cand indexam pozitiile nu "ciclam" pe lista circulara ca la
 * get, ci consideram nodurile in ordinea de la head la ultimul (adica acel nod
 * care pointeaza la head ca nod urmator in lista). Daca n >= nr_noduri, atunci
 * adaugam nodul nou la finalul listei. Daca n < 0, eroare.
 */
void
dll_add_nth_node(doubly_linked_list_t* list, unsigned int n, const void* data)
{
	dll_node_t *node,*aux;
	unsigned int i;
	DIE(n<0,"wrong place");
	DIE(list==NULL,"got no list");

	node=malloc(sizeof(dll_node_t));
	DIE(node==NULL,"malloc dll_node_t failed");

	node->data=malloc(list->data_size);
	DIE(node->data==NULL,"malloc node->data failed");

	memcpy(node->data,data,list->data_size);

	if(n>list->size)
		n=list->size;

	if(n==0 || list->size==0)
	{
		node->next=list->head;
		list->head=node;
		if(node->next==NULL)
		{
			node->next=node->prev=node;
		}
		else
		{
			node->prev=node->next->prev;
			node->prev->next=node;
			node->next->prev=node;
		}
	}
	else
	{
		i=1;
		for(aux=list->head;i<n;i++)	aux=aux->next;
		node->next=aux->next;
		node->prev=aux;
		aux->next=node;
		node->next->prev=node;
	}
	list->size++;
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Functia intoarce un pointer spre acest nod
 * proaspat eliminat din lista. Daca n >= nr_noduri - 1, se elimina nodul de la
 * finalul listei. Daca n < 0, eroare. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */
dll_node_t*
dll_remove_nth_node(doubly_linked_list_t* list, unsigned int n)
{
	dll_node_t *node;
	unsigned int i;
	DIE(n<0,"wrong place");
	DIE(list==NULL,"got no list");
	DIE(list->head==NULL,"list is empty");

	if(n>list->size)
		n=list->size-1;

	i=0;
	for(node=list->head;i<n;i++)	node=node->next;
	node->prev->next=node->next;
	node->next->prev=node->prev;

	if(node==list->head)
		list->head=list->head->next;

	if(list->head==node)
		list->head=NULL;

	node->next=NULL;
	node->prev=NULL;
	list->size--;
	return node;
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int
dll_get_size(doubly_linked_list_t* list)
{
	dll_node_t *aux;
	int size=1;
	DIE(list==NULL,"got no list");

	if(list->head==NULL)
		return 0;
	for(aux=list->head;aux->next!=list->head;aux=aux->next) size++;
	return size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista.
 */
void
dll_free(doubly_linked_list_t** pp_list)
{
	dll_node_t *aux;
	DIE(pp_list==NULL,"got no pointer to list");
	DIE(pp_list[0]==NULL,"got no list");

	if(pp_list[0]->head!=NULL)
		pp_list[0]->head->prev->next=NULL;

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
 * din lista separate printr-un spatiu, incepand de la primul nod din lista.
 */
void
dll_print_int_list(doubly_linked_list_t* list)
{
	dll_node_t *aux;
	DIE(list==NULL,"got no list");

	if(list->head!=NULL)
	{
		printf("%d ",((int*)(list->head->data))[0]);
		for(aux=list->head->next;aux!=list->head;aux=aux->next)
			printf("%d ",((int*)(aux->data))[0]);
	}
    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista separate printr-un spatiu, incepand de la primul nod din
 * lista.
 */
void
dll_print_string_list(doubly_linked_list_t* list)
{
	dll_node_t *aux;
	DIE(list==NULL,"got no list");

	if(list->head!=NULL)
	{
		printf("%s ",((char*)(list->head->data)));
		for(aux=list->head->next;aux!=list->head;aux=aux->next)
			printf("%s ",((char*)(aux->data)));
	}
    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza o singura data toate valorile int
 * stocate in nodurile din lista, separate printr-un spatiu, incepand de la
 * nodul dat ca parametru si continuand la stanga in lista dublu inlantuita
 * circulara, pana cand sunt afisate valorile tuturor nodurilor.
 */
void
dll_print_ints_left_circular(dll_node_t* start)
{
	dll_node_t *aux;
	DIE(start==NULL,"got no node");

	printf("%d ",((int*)(start->data))[0]);
	for(aux=start->next;aux!=start;aux=aux->prev)
		printf("%d ",((int*)(aux->data))[0]);
    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza o singura data toate valorile int
 * stocate in nodurile din lista, separate printr-un spatiu, incepand de la
 * nodul dat ca parametru si continuand la dreapta in lista dublu inlantuita
 * circulara, pana cand sunt afisate valorile tuturor nodurilor.
 */
void
dll_print_ints_right_circular(dll_node_t* start)
{
	dll_node_t *aux;
	DIE(start==NULL,"got no node");

	printf("%d ",((int*)(start->data))[0]);
	for(aux=start->next;aux!=start;aux=aux->next)
		printf("%d ",((int*)(aux->data))[0]);
    printf("\n");
}
