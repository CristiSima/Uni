#include <stdlib.h>

#include "Stack.h"
#include "utils.h"

stack_t *
st_create(unsigned int data_size)
{
	stack_t *stack=malloc(sizeof(stack_t));

	stack->list=ll_create(data_size);

	return stack;
}

unsigned int
st_get_size(stack_t *st)
{
	if(st==NULL)
		return 0;

	return st->list->size;
}

/*
 * Intoarce 1 daca stiva este goala si 0 in caz contrar.
 */
unsigned int
st_is_empty(stack_t *st)
{
	if(st==NULL)
		return 0;

	return st->list->size==0;
}

void *
st_peek(stack_t *st)
{
	if(st==NULL)
		return NULL;

	return st->list->head->data;
}

void
st_pop(stack_t *st)
{
	if(st==NULL)
		return;

	ll_node_t *node=ll_remove_nth_node(st->list,0);
	free(node->data);
	free(node);
}

void
st_push(stack_t *st, void *new_data)
{
	if(st==NULL)
		return;

	ll_add_nth_node(st->list,0,new_data);
}

void
st_clear(stack_t *st)
{
	ll_node_t *node;
	if(st==NULL)
		return;

	while(st->list->size)
	{
		node=ll_remove_nth_node(st->list,0);
		free(node->data);
		free(node);
	}
}

void
st_free(stack_t *st)
{
	if(st==NULL)
		return;
		
	ll_free(&st->list);
	free(st);
}
