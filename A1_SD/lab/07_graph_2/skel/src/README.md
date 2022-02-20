# Test.c

## dfs_connected_comps
```c
static void dfs_connected_comps(list_graph_t *lg, int node, int *visited, linked_list_t *component)
```
adds the current node to the `component` list and calls itself for all it's neighbors that haven't been visited

## connected_components
```c
static linked_list_t **connected_components(list_graph_t *lg, int *visited, unsigned int *num_comp)
```
calls [`dfs_connected_comps`](#dfs_connected_comps) on all non visited nodes<BR>
and returns a vector of lists of different connected components

## dfs_topo_sort
```c
static void dfs_topo_sort(list_graph_t *lg, int node, int *visited, linked_list_t *sorted)
```
calls itself on all non visited neighbors<BR>
then adds itself to the beginning of `sorted`

## topo_sort
```c
static linked_list_t *topo_sort(list_graph_t *lg, int *visited)
```
calls [`dfs_topo_sort`](#dfs_topo_sort) on all non visited nodes

## min_path
```c
static void min_path(list_graph_t *lg, int start, int *dist)
```
does a BFS from `start` but only goes to a node if the previously calculated distance is greater then `distance[current]+1`

## min_path_origin
```c
static void min_path_origin(list_graph_t *lg, int start, int *dist, int *origin)
```
does a BFS from `start` but only goes to a node if the previously calculated distance is greater then `distance[current]+1`<BR>
saves the node it went from in `origin`

## check_bipartite
```c
static int check_bipartite(list_graph_t *lg, int *color)
```
uses a BFS to traverse the graph adding all the neighbors of a node the the other color<BR>
while checking for a neighbor with the same color
at the end checks if all the nodes have been visited

## dfs_all_paths
```c
void dfs_all_paths(list_graph_t *lg, int node, int *visited, linked_list_t *path)
```
calls itself on all non visited neighbors, but only keeps visited for the subsequent calls
If it reaches node 0 having visited all nodes prints all of `path`<BR>
`path` mirrors all `node` of functions on the callstack

## Problem1
```c
void Problem1(void)
```
reads the graph and prints the clans<BR>
uses: [`connected_components`](#connected_components)

## Problem2_old
```c
void Problem2_old(void)
```
reads the graph and prints the shortest path<BR>
the shortest path is calculated by continuously choosing a node closer to the start from the end<BR>
uses: [`min_path`](#min_path)

## Problem2
```c
void Problem2(void)
```
reads the graph and prints the shortest path<BR>
the shortest path is calculated by going back from the end using `origin`<BR>
uses: [`min_path_origin`](#min_path_origin)

## Problem4
```c
void Problem4(void)
```
reads the graph and checks if it's bipartite, printing the partitions if so<BR>
uses: [`check_bipartite`](#check_bipartite)

## Problem5
```c
void Problem5(void)
```
reads the graph and calls [`dfs_all_paths`](#dfs_all_paths) from node 0

## Note:
most functions use the macro:
```c
#define c_neighbor ((int*)(aux->data))[0]
```
that is undefined at the end of the function to prevent conflicts
