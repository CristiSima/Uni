# avl.c

## __avl_fix
```c
static void __avl_fix(avl_node_t **node)
```
uses `__rotate_right` and `__rotate_left` to rearrange the tree

## __avl_insert
```c
static void __avl_insert(avl_node_t **node,
						void *value, size_t data_size,
						int (*cmp)(const void*, const void*))
```
calls itself until it reaches the end of a branch then adds the node<BR>
when returning recalculates the height and fixes the tree

## __get_key
```c
static int __get_key(avl_node_t *node, void *value,
					int (*cmp)(const void*, const void*))
```
calls itself until it reaches the searched node<BR>

## __avl_delete
```c
static void __avl_delete(avl_node_t **node, void *value,
						int (*cmp)(const void*, const void*))
```
calls itself until it reaches the searched node<BR>
if it has 2 children the data is swapped with the maximum of the left subtree and removed it<BR>
if it has 1 child then it's skipped<BR>
if it is a leaf then it's removed

<BR>
<BR>

# rb_tree.c

## rb_tree_create
```c
rb_tree_t *rb_tree_create(int (*f) (const void *, const void *),
						  size_t ksz, size_t dsz)
```
allocates memory and initializes values

## __swap_color
```c
static void __swap_color(rb_node_t *node_a,
						rb_node_t *node_b)
```
swaps colors

## __rb_tree_rotate_right
```c
static void __rb_tree_rotate_right(rb_tree_t *rb_tree,
									rb_node_t *rb_node)
```
rotates the subtree

## __rb_tree_insert_fix
```c
static void __rb_tree_insert_fix(rb_tree_t *rb_tree,
	 							rb_node_t *rb_node)
```
walks up the tree while both `rb_node` and it's parent are RED and rotates the subtree
