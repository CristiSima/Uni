#include "LinkedList.h"

void ll_remove_duplicates(linked_list_t *list);

// circular:the last node points to the first
void ll_make_circular(linked_list_t *list);

int ll_is_circular(linked_list_t *list);

void ll_unmake_circular(linked_list_t *list);

// cicle:the last node points to another inside the list 
void ll_make_cicle(linked_list_t *list,int n);

int ll_has_cicle(linked_list_t *list);

void ll_unmake_cicle(linked_list_t *list);


ll_node_t *get_nth_node(linked_list_t *list,int n);

ll_node_t *get_last_nth_node(linked_list_t *list,int n);
