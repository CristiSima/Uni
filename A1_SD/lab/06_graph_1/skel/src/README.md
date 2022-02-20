# ListGraph.c

## lg_create
```c
list_graph_t *lg_create(int nodes)
```
allocates memory and creates the `->neighbors` lists

## lg_add_edge
```c
void lg_add_edge(list_graph_t* graph, int src, int dest)
```
uses `ll_add_nth_node`

## lg_has_edge
```c
int lg_has_edge(list_graph_t* graph, int src, int dest)
```
iterates over the list of `src`

## lg_get_neighbours
```c
linked_list_t *lg_get_neighbours(list_graph_t* graph, int node)
```
returns the list of `node`

## lg_remove_edge
```c
void lg_remove_edge(list_graph_t* graph, int src, int dest)
```
searches in the list of `src` and `dest` for a node with the other value and removes it using `ll_remove_nth_node`

## lg_free
```c
void lg_free(list_graph_t* graph)
```
frees the list and then the rest of the memory

<BR>
<BR>

# MatrixGraph.c

## mg_create
```c
matrix_graph_t *mg_create(int nodes)
```
allocates memory

## mg_add_edge
```c
void mg_add_edge(matrix_graph_t* graph, int src, int dest)
```
sets `->matrix[src][dest]` and `->matrix[dest][src]`

## mg_has_edge
```c
int mg_has_edge(matrix_graph_t* graph, int src, int dest)
```
returns `->matrix[src][dest]`

## mg_remove_edge
```c
void mg_remove_edge(matrix_graph_t* graph, int src, int dest)
```
resets `->matrix[src][dest]` and `->matrix[dest][src]`

## mg_free
```c
void mg_free(matrix_graph_t* graph)
```
frees memory

<BR>
<BR>

# Test.c

## print_matrix_graph
```c
void print_matrix_graph(matrix_graph_t* mg)
```
prints a matrix

## print_list_graph
```c
void print_list_graph(list_graph_t* lg)
```
uses `ll_print_int` for all lists adding `"node_number: "` before

## dfs_list_graph
```c
void dfs_list_graph(list_graph_t* lg, int node, int* visited, int* st_node, int* st_iter)
```
uses `st_node` and `st_iter` to simulate a callstack containing a node and index of the element in that node's neighbor list that was reached

## dfs_matrix_graph
```c
void dfs_matrix_graph(matrix_graph_t* mg, int node, int* visited, int* st_node, int* st_iter)
```
uses `st_node` and `st_iter` to simulate a callstack containing a node and iterator that searches for neighbors

## bfs_list_graph
```c
void bfs_list_graph(list_graph_t* lg, int node, int* color, int* parent)
```
uses a queue to store the nodes that will be visited

## bfs_matrix_graph
```c
void bfs_matrix_graph(matrix_graph_t* mg, int node, int* color, int* parent)
```
uses a queue to store the nodes that will be visited

## floyd_warshall
```c
void floyd_warshall(matrix_graph_t* mg)
```
for every node (`k`) checks if in can be used as a shortcut or path between any other 2 (`i` and `j`) <BR>
there needs to be a path between (`i`,`k`) and (`k`,`j`)
