# bst.c

## __bst_node_create
```c
static bst_node_t *__bst_node_create(void *data, size_t data_size)
```
allocates memory for a node and copies the data in it  

## bst_tree_create
```c
bst_tree_t *bst_tree_create(size_t data_size, int (*cmp_f)(const void *, const void *))
```
allocates memory for the tree and initialises it

## bst_tree_insert
```c
void bst_tree_insert(bst_tree_t *bst_tree, void *data)
```
walks on the tree until it finds a place to place the new node

## __bst_tree_remove
```c
static bst_node_t *__bst_tree_remove(bst_node_t *bst_node,
                                    void *data, size_t data_size,
                                    int (*cmp)(const void *, const void *),
									void (*free_data)(void *))
```
walks on the tree until it finds the node(by calling itself)<BR>
once it found the node that needs to be removed:
1. gets the biggest node smaller then itself or the right child if it doesn't have a left child
2. that node is removed from the tree by calling itself
3. then replaces the original removed node

in order to maintain the links the function returns the root of the curent subtree

## bst_tree_remove
```c
void bst_tree_remove(bst_tree_t *bst_tree, void *data, void (*free_data)(void *))
```
wrapper for [__bst_tree_remove](#__bst_tree_remove)

## __bst_tree_free
```c
static void __bst_tree_free(bst_node_t *bst_node, void (*free_data)(void *))
```
calls itself on the children of `bst_node` then frees the memory

## bst_tree_free
```c
void bst_tree_free(bst_tree_t *bst_tree, void (*free_data)(void *))
```
calls [__bst_tree_free](#__bst_tree_free)

## __bst_tree_print_inorder
```c
static void __bst_tree_print_inorder(bst_node_t* bst_node, void (*print_data)(void*))
```
calls itself for the left and right nodes printing the data<BR>
**L**eft **D**ata **R**ight

## bst_tree_print_inorder
```c
void bst_tree_print_inorder(bst_tree_t* bst_tree, void (*print_data)(void*))
```
uses [__bst_tree_print_inorder](#__bst_tree_print_inorder) to print the tree<BR>
**L**eft **D**ata **R**ight

<BR>
<BR>

# heap.c

## heap_create
```c
heap_t *heap_create(int (*cmp_f)(const team_t *, const team_t *))
```
allocates memory for a node and copies the data in it

## __heap_insert_fix
```c
static void __heap_insert_fix(heap_t *heap, int pos)
```
moves up the tree the node on `pos`

## heap_insert
```c
void heap_insert(heap_t *heap, team_t *team)
```
adds the team at the end of the heap then calls [__heap_pop_fix](#__heap_pop_fix)<BR>
extends the heap if at capacity

## heap_top
```c
team_t *heap_top(heap_t *heap)
```
returns the element on top of the heap

## __heap_pop_fix
```c
static void __heap_pop_fix(heap_t *heap, int pos)
```
switches places with the child who has the propriety then calls itself on that child<BR>
once it reached a leaf moves the leafs after it and calls [__heap_insert_fix](#__heap_insert_fix) on them

## heap_pop
```c
void heap_pop(heap_t *heap)
```
frees the top and calls [__heap_pop_fix](#__heap_pop_fix)

## heap_empty
```c
int heap_empty(heap_t *heap)
```
check if the heap is empty

## heap_free
```c
void heap_free(heap_t *heap)
```
iterates over `heap->arr` and frees the memory
