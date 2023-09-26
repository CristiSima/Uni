#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H 1

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct ll_node
{
    void *data;
    struct ll_node *next;
};

typedef struct {
    struct ll_node *front;
    struct ll_node *end;
} linked_list;

#define ll_iterator(list, iterator) for(iterator=list.front;iterator;iterator=iterator->next)
#define ll_iterator_def(list, iterator) for(struct ll_node *iterator=list.front;iterator;iterator=iterator->next)

#define ll_iterator_p(list, iterator) for(iterator=list->front;iterator;iterator=iterator->next)
#define ll_iterator_p_def(list, iterator) for(struct ll_node *iterator=list->front;iterator;iterator=iterator->next)

linked_list ll_create();

linked_list *ll_create_p();

void ll_add_front_p(linked_list *list, void *data);
#define ll_add_front(list, data) ll_add_front_p(&(list), data)

void ll_add_end_p(linked_list *list, void *data);
#define ll_add_end(list, data) ll_add_end_p(&(list), data)

void *ll_pop_front_p(linked_list *list);

#define ll_pop_front(list) ll_pop_front_p(&(list))


#define ll_pop_front_p_instance(name, val_type) val_type *name(linked_list *list) {return ll_pop_front_p(list);}

#define dereference_to(data, type) (*(type*)(data))

linked_list *ll_free_p(linked_list *list, void free_func(void*));

#define ll_free(list, free_func) ll_free_p(&(list), free_func)

#endif // _LINKED_LIST_H
