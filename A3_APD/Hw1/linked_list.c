#include "linked_list.h"

linked_list ll_create()
{
    return (linked_list){.front=NULL, .end=NULL};
}

linked_list *ll_create_p()
{
    return calloc(1, sizeof(linked_list));
}

void ll_add_front_p(linked_list *list, void *data)
{
    struct ll_node *node=calloc(1, sizeof(struct ll_node));
    node->data=data;

    node->next=list->front;

    if(!list->front)
        list->front=list->end=node;
}

void ll_add_end_p(linked_list *list, void *data)
{
    struct ll_node *node=calloc(1, sizeof(struct ll_node));
    node->data=data;


    if(list->front)
    {
        list->end->next=node;
        list->end=node;
    }
    else
        list->front=list->end=node;
}

void *ll_pop_front_p(linked_list *list)
{
    if(list->front==NULL)
        return NULL;

    struct ll_node *node;

    node=list->front;

    list->front=list->front->next;

    if(list->end==node)
        list->end=list->front;

    void *data=node->data;
    free(node);
    return data;
}

linked_list *ll_free_p(linked_list *list, void free_func(void*))
{
    for(void *data=ll_pop_front_p(list); data; data=ll_pop_front_p(list))
        free_func(data);

    list->front=list->end=NULL;
    return list;
}
