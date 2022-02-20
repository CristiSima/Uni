#include "E2.h"
#include <stdlib.h>

void ll_rearrange_ltx(linked_list_t *list,int x)
{
	ll_node_t *aux,*node;
	for(aux=list->head;aux->next!=NULL;aux=aux->next)
	{
		while(aux->next!=NULL && ((int*)(aux->next->data))[0]<=x)
		{
			node=aux->next;
			aux->next=node->next;
			node->next=list->head;
			list->head=node;
		}
	}
}

int main()
{
	linked_list_t *list=ll_create(sizeof(int));
	int nr;
	nr=7;ll_add_nth_node(list,10,&nr);
	nr=3;ll_add_nth_node(list,10,&nr);
	nr=5;ll_add_nth_node(list,10,&nr);
	nr=2;ll_add_nth_node(list,10,&nr);
	nr=8;ll_add_nth_node(list,10,&nr);
	nr=1;ll_add_nth_node(list,10,&nr);
	nr=9;ll_add_nth_node(list,10,&nr);
	
	ll_print_int(list);
	ll_rearrange_ltx(list,5);
	ll_print_int(list);
	ll_free(&list);
	return 0;
}
// gcc E2.c LinkedList.c -o E2;./E2;rm E2
