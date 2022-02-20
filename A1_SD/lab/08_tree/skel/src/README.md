# binary_tree.c

## b_tree_create
```c
b_tree_t *b_tree_create(size_t data_size)
```
allocates and initialises memory

## b_tree_insert
```c
void b_tree_insert(b_tree_t* b_tree, void* data)
```
creates a node and adds it to the first free position<BR>
the search is done using a BFS

## __b_tree_print_preorder
```c
static void __b_tree_print_preorder(b_node_t* b_node, void (*print_data)(void*))
```
**INTERNAL**<BR>
calls itself for the left and right nodes printing the data<BR>
**D**ata **L**eft **R**ight

## b_tree_print_preorder
```c
void b_tree_print_preorder(b_tree_t* b_tree, void (*print_data)(void*))
```
uses [`__b_tree_print_preorder`](#__b_tree_print_preorder) to print the tree<BR>
**D**ata **L**eft **R**ight


## __b_tree_print_inorder
```c
static void __b_tree_print_inorder(b_node_t* b_node, void (*print_data)(void*))
```
**INTERNAL**<BR>
calls itself for the left and right nodes printing the data<BR>
**L**eft **D**ata **R**ight

## b_tree_print_inorder
```c
void b_tree_print_inorder(b_tree_t* b_tree, void (*print_data)(void*))
```
uses [`__b_tree_print_inorder`](#__b_tree_print_inorder) to print the tree<BR>
**L**eft **D**ata **R**ight

## __b_tree_print_postorder
```c
static void __b_tree_print_postorder(b_node_t* b_node, void (*print_data)(void*))
```
**INTERNAL**<BR>
calls itself for the left and right nodes printing the data<BR>
**L**eft **R**ight **D**ata

## b_tree_print_postorder
```c
void b_tree_print_postorder(b_tree_t* b_tree, void (*print_data)(void*))
```
uses [`__b_tree_print_postorder`](#__b_tree_print_postorder) to print the tree<BR>
**L**eft **R**ight **D**ata

## __b_tree_free
```c
static void __b_tree_free(b_node_t* b_node, void (*free_data)(void*))
```
**INTERNAL**<BR>
calls itself for the left and right nodes freeing the data

## b_tree_free
```c
void b_tree_free(b_tree_t* b_tree, void (*free_data)(void*))
```
uses [`__b_tree_free`](#__b_tree_free) to free the nodes

## b_tree_height
```c
int b_tree_height(b_tree_t *b_tree)
```
always goes to the left and counts the nodes

<BR>
<BR>

# task2.c

## __calc_max_path
```c
static void __calc_max_path(b_node_t* b_node)
```
**INTERNAL**<BR>
calls itself until it reaches the leafs<BR>
when returning adds to the current node the maximum of its children<BR>
uses a macro to cast the data in a node
```c
#define data_int(node) ((int*)node->data)[0]
```


## calc_max_path
```c
void calc_max_path(b_tree_t* b_tree)
```
uses [`__calc_max_path`](#__calc_max_path) to calculate the maximum sum of any path

## __print_stack
```c
static void __print_stack(int *stack, int depth)
```
**INTERNAL**<BR>
prinst `depth` elements from `stack`

## __print_max_path
```c
static void __print_max_path(b_node_t* b_node, int *stack, int depth)
```
**INTERNAL**<BR>
calls itself on the children of a node with the bigger value, both if equal<BR>
once it reaches the leafs uses [`__print_stack`](#__print_stack) to print the path

## print_max_path
```c
void print_max_path(b_tree_t* b_tree)
```
wrapper for [`__print_max_path`](#__print_max_path)<BR>
expects a tree processed by [`calc_max_path`](#calc_max_path)

<BR>
<BR>

# bonus.c

## b_tree_nice_free
```c
void b_tree_nice_free(b_tree_t* b_tree, void (*free_data)(void*))
```
moves the right subtree to the most left position then frees the root, the left subtree become the new tree <BR>
this is repeats until everything is freed
