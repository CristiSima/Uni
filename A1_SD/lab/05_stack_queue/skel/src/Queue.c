#include <stdlib.h>
#include <string.h>

#include "Queue.h"
#include "utils.h"

queue_t *
q_create(unsigned int data_size, unsigned int max_size)
{
	queue_t *queue=malloc(sizeof(queue_t));
	
	queue->max_size=max_size;
	queue->size=0;
	queue->data_size=data_size;
	queue->read_idx=0;
	queue->write_idx=0;
	queue->buff=malloc(sizeof(void*)*max_size);

	return queue;
}

unsigned int
q_get_size(queue_t *q)
{
	if(q==NULL)
		return 0;

	return q->size;
}

/*
 * Intoarce 1 daca stiva este goala si 0 in caz contrar.
 */
unsigned int
q_is_empty(queue_t *q)
{
	if(q==NULL)
		return 0;

	return q->size==0;
}

void *
q_front(queue_t *q)
{
	if(q==NULL)
		return NULL;

	return q->buff[q->read_idx];
}

/* Functia intoarce true daca operatia s-a efectuat si false in caz contrar */
bool
q_dequeue(queue_t *q)
{
	if(q==NULL)
		return false;
	if(q->size==0)
		return false;

	free(q->buff[q->read_idx]);
	q->read_idx=(q->read_idx+1)%q->max_size;
	q->size--;

	return true;
}

/* Functia intoarce true daca operatia s-a efectuat si false in caz contrar */
bool
q_enqueue(queue_t *q, void *new_data)
{
	if(q==NULL)
		return false;
	if(q->size==q->max_size)
		return false;

	q->buff[q->write_idx]=malloc(q->data_size);
	memcpy(q->buff[q->write_idx],new_data,q->data_size);
	q->write_idx=(q->write_idx+1)%q->max_size;
	q->size++;

	return true;
}

void
q_clear(queue_t *q)
{
	if(q==NULL)
		return;

	while(q_dequeue(q));
}

void
q_free(queue_t *q)
{
	if(q==NULL)
		return;

	while(q_dequeue(q));
	free(q->buff);
	free(q);
}
