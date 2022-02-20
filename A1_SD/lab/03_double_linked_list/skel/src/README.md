# CircularDoublyLinkedList.c

## dll_create
```c
doubly_linked_list_t *dll_create(unsigned int data_size)
```
allocates memory and initializes

## dll_get_nth_node
```c
dll_node_t *dll_get_nth_node(doubly_linked_list_t* list, unsigned int n)
```
iterates n times and returns the selected node

## dll_add_nth_node
```c
void dll_add_nth_node(doubly_linked_list_t* list, unsigned int n, const void* data)
```
creates a new node(copying the data in it)<BR>
if n or the list's size are 0 that node becomes the new head<BR>
otherwise the last node or the one before the nth is selected and the new node is inserted afterwards

## dll_remove_nth_node
```c
dll_node_t *dll_remove_nth_node(doubly_linked_list_t* list, unsigned int n)
```
iterates n times, remove the selected node from the list and return it

## dll_get_size
```c
unsigned int dll_get_size(doubly_linked_list_t* list)
```
iterates over the list

## dll_free
```c
void dll_free(doubly_linked_list_t** pp_list)
```
iterates over the list to free the data in the nodes,and the nodes<BR>
then frees the list and sets the pointer to the freed list to NULL

## dll_print_int_list/dll_print_string_list
```c
void dll_print_int_list(doubly_linked_list_t* list);
void dll_print_string_list(doubly_linked_list_t* list);
```
iterates until it reaches ``list->head`` and prints unsing ``%d`` and ``%s``

## dll_print_ints_left_circular/dll_print_ints_right_circular
```c
void dll_print_ints_left_circular(dll_node_t* start);
void dll_print_ints_right_circular(dll_node_t* start);
```
iterates until it reaches ``start`` and prints using ``%d``

<BR>
<BR>

# E2.c

## ll_is_palindrome
```c
int ll_is_palindrome(doubly_linked_list_t* list)
```
simultaneously iterates from the start and end of the list<br>
and compares the first byte of ``->data`` until ``start`` and ``end`` meet

## gen_str_dll
```c
doubly_linked_list_t *gen_str_dll(char *str)
```
makes a ``doubly_linked_list_t`` with one char per string out of a string

<BR>
<BR>

# E3.c

## removeNthFromEnd
```c
struct ListNode* removeNthFromEnd(struct ListNode* head, int n);
```
the function iterates over the list only once using recursion<BR>
the function recurses until it reaches the end and return a node that points to itself<BR>
using the ``val`` field instances of the function can communicate the distance to the end of the list<BR>
when the nth element is reached the function frees the cyclic list and returns the n-1th  element<BR>
the rest of the instances make sure that the next instances ``->next`` is the value returned.
