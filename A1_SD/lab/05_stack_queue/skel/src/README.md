# Queue.c

## q_create
```c
queue_t *q_create(unsigned int data_size, unsigned int max_size)
```
alocates memory and initializes variables

## q_get_size
```c
unsigned int q_get_size(queue_t *q)
```
returns `q->size`

## q_is_empty
```c
unsigned int q_is_empty(queue_t *q)
```
returns `q->size==0`

## q_front
```c
void *q_front(queue_t *q)
```
returns a pointer to the first element

## q_dequeue
```c
bool q_dequeue(queue_t *q)
```
remove the first element freeing the memory used by it

## q_enqueue
```c
bool q_enqueue(queue_t *q, void *new_data)
```
alocates memory and copy the value in

## q_clear
```c
void q_clear(queue_t *q)
```
clears the queue by repeating `q_dequeue`

## q_free
```c
void q_free(queue_t *q)
```
clears the queue by repeating `q_dequeue` and frees all the memory

<BR>
<BR>

# Stack.c

## st_create
```c
stack_t *st_create(unsigned int data_size)
```
alocates memory and initializes variables

## st_get_size
```c
unsigned int st_get_size(stack_t *st)
```
returns `st->list->size`

## st_is_empty
```c
unsigned int st_is_empty(stack_t *st)
```
returns `st->list->size==0`

## st_peek
```c
void *st_peek(stack_t *st)
```
returns a pointer to the first element

## st_pop
```c
void st_pop(stack_t *st)
```
removes the first element

## st_push
```c
void st_push(stack_t *st, void *new_data)
```
pushes element to the stack

## st_clear
```c
void st_clear(stack_t *st)
```
clears the queue by repeatedly removeing the first element

## st_free
```c
void st_free(stack_t *st)
```
uses `ll_free` 
