# treap.c

## rotate_right
```c
void rotate_right(Node** node);
//      node                lson
//      /  \                /  \
//   lson   y     --->     x   node
//   /  \                /   \
//  x    lrson         lrson   y
```
rotates the tree

## rotate_left
```c
void rotate_left(Node** node);
//        node            rson
//       /  \             /  \
//      x    rson --->  node  y
//    /   \            /   \
//  rlson  y          x   rlson
```
rotates the tree

## treap_insert
```c
void treap_insert(Node** node,
				  void* value,
				  int data_size,
				  int (*compar)(void*, void*))
```
recursively walks on the tree to find where to add a node<BR>
and rearranges the tree when returning

## treap_delete
```c
void treap_delete(Node** node,
				  void* value,
				  int data_size,
				  int (*compar)(void*, void*))
```
recursively walks on the tree to find the node that needs to be deleted<BR>
then it's pushed down the tree with rotations until it becomes a leaf and finally removed

## get_key
```c
void *get_key(Node* node, void* value,
			  int data_size, int (*compar)(void*, void*))
```
recursively walks on the tree to find the node then returns `node->data`

<BR>
<BR>

# main.c

## ascending_nodes
```c
void ascending_nodes(Node* node, int* keys, int* num_keys)
```
recursively walks on the tree adding `->data` to `num_keys`<BR>
