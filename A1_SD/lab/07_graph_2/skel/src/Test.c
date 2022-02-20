#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ListGraph.h"
#include "Queue.h"
#include "Stack.h"
#include "utils.h"


#define MAX_NODES	100
#define INF		0x3f3f3f3f

/**
 * Adaugati nodurile componentei conexe curente in lista component.
 */
static void
dfs_connected_comps(list_graph_t *lg, int node, int *visited, linked_list_t *component)
{
	#define c_neighbor ((int*)(aux->data))[0]

	visited[node]=1;
	ll_add_nth_node(component,component->size,&node);

	for(ll_node_t *aux=lg->neighbors[node]->head;aux!=NULL;aux=aux->next)
		if(!visited[c_neighbor])
			dfs_connected_comps(lg,c_neighbor,visited,component);

	#undef c_neighbor
}

static linked_list_t **
connected_components(list_graph_t *lg, int *visited, unsigned int *num_comp)
{
	num_comp[0]=0;
	linked_list_t **comps = malloc(lg->nodes * sizeof(*comps));
	DIE(!comps, "malloc comps failed");

	for(int i=0;i<lg->nodes;i++)
		if(!visited[i])
		{
			comps[num_comp[0]]=ll_create(sizeof(int));
			dfs_connected_comps(lg,i,visited,comps[num_comp[0]]);
			num_comp[0]++;
		}

	return comps;
}

static void
dfs_topo_sort(list_graph_t *lg, int node, int *visited, linked_list_t *sorted)
{
	#define c_neighbor ((int*)(aux->data))[0]

	visited[node]=1;

	for(ll_node_t *aux=lg->neighbors[node]->head;aux!=NULL;aux=aux->next)
		if(!visited[c_neighbor])
			dfs_topo_sort(lg,c_neighbor,visited,sorted);

	ll_add_nth_node(sorted,0,&node);

	#undef c_neighbor
}

static linked_list_t *
topo_sort(list_graph_t *lg, int *visited)
{
	linked_list_t *sorted = ll_create(sizeof(int));

	for(int i=0;i<lg->nodes;i++)
		if(!visited[i])
			dfs_topo_sort(lg,i,visited,sorted);

	return sorted;
}

static void
min_path_origin(list_graph_t *lg, int start, int *dist, int *origin)
{
	#define c_neighbor ((int*)(aux->data))[0]

	int current;
	queue_t *q=q_create(sizeof(int),lg->nodes);

	dist[start]=0;
	q_enqueue(q,&start);

	do
	{
		current=((int*)q_front(q))[0];
		q_dequeue(q);

		for(ll_node_t *aux=lg->neighbors[current]->head;aux!=NULL;aux=aux->next)
			if(dist[c_neighbor]>dist[current]+1)
			{
				q_enqueue(q,aux->data);
				dist[c_neighbor]=dist[current]+1;
				origin[c_neighbor]=current;
			}
	}
	while(!q_is_empty(q));

	q_free(q);

	#undef c_neighbor
}

static void
min_path(list_graph_t *lg, int start, int *dist)
{
	#define c_neighbor ((int*)(aux->data))[0]

	int current;
	queue_t *q=q_create(sizeof(int),lg->nodes);

	dist[start]=0;
	q_enqueue(q,&start);

	do
	{
		current=((int*)q_front(q))[0];
		q_dequeue(q);

		for(ll_node_t *aux=lg->neighbors[current]->head;aux!=NULL;aux=aux->next)
			if(dist[c_neighbor]>dist[current]+1)
			{
				q_enqueue(q,aux->data);
				dist[c_neighbor]=dist[current]+1;
			}
	}
	while(!q_is_empty(q));

	q_free(q);

	#undef c_neighbor
}

static int
check_bipartite(list_graph_t *lg, int *color)
{
	#define c_neighbor ((int*)(aux->data))[0]

	int current=0;
	queue_t *q=q_create(sizeof(int),lg->nodes);

	color[0]=1;
	q_enqueue(q,&current);

	do
	{
		current=((int*)q_front(q))[0];
		q_dequeue(q);

		for(ll_node_t *aux=lg->neighbors[current]->head;aux!=NULL;aux=aux->next)
			if(color[c_neighbor]==color[current])
			{
				q_free(q);
				return 0;
			}
			else if(color[c_neighbor]==0)
			{
				q_enqueue(q,aux->data);

				if(color[current]==1)
					color[c_neighbor]=2;
				else
					color[c_neighbor]=1;
			}
	}
	while(!q_is_empty(q));

	q_free(q);

	for(int i=0;i<lg->nodes;i++)
		if(!color[i])
			return 0;

	return 1;

	#undef c_neighbor
}

void dfs_all_paths(list_graph_t *lg, int node, int *visited, linked_list_t *path)
{
	#define c_neighbor ((int*)(aux->data))[0]

	ll_node_t *free_node;

	for(ll_node_t *aux=lg->neighbors[node]->head;aux!=NULL;aux=aux->next)
		if(!visited[c_neighbor])
		{
			visited[c_neighbor]=1;
			ll_add_nth_node(path,path->size,aux->data);

			dfs_all_paths(lg,c_neighbor,visited,path);

			free_node=ll_remove_nth_node(path,path->size-1);//-1?
			free(free_node->data);
			free(free_node);

			visited[c_neighbor]=0;
		}
		else if(c_neighbor==0 && path->size==lg->nodes)
			ll_print_int(path);

	#undef c_neighbor
}
static void
test_connected_comp(void)
{
	unsigned int num_comps, i, nodes, edges;
	int x, y;
	int visited[MAX_NODES] = {0};
	list_graph_t *lg;
	linked_list_t **comps;

	printf("UNDIRECTED graph for the connected components problem:\n");
	scanf("%u %u", &nodes, &edges);
	lg = lg_create(nodes);

	for (i = 0; i != edges; ++i) {
		scanf("%d %d", &x, &y);
		lg_add_edge(lg, x, y);
		lg_add_edge(lg, y, x);
	}

	comps = connected_components(lg, visited, &num_comps);

	printf("Found %d connected components:\n", num_comps);
	for (i = 0; i != num_comps; ++i) {
		ll_print_int(comps[i]);
		ll_free(comps + i);
	}
	printf("\n");

	free(comps);
	lg_free(lg);
}

static void
test_topo_sort(void)
{
	unsigned int i, nodes, edges;
	int x, y;
	int visited[MAX_NODES] = {0};
	list_graph_t *lg;
	linked_list_t *sorted;

	printf("DIRECTED graph for the topological sort problem:\n");
	scanf("%u %u", &nodes, &edges);
	lg = lg_create(nodes);

	for (i = 0; i != edges; ++i) {
		scanf("%d %d", &x, &y);
		lg_add_edge(lg, x, y);
	}

	sorted = topo_sort(lg, visited);

	printf("Topologically sorted nodes:\n");
	ll_print_int(sorted);
	printf("\n");

	ll_free(&sorted);
	lg_free(lg);
}

static void
test_min_dist(void)
{
	unsigned int i, nodes, edges;
	int x, y;
	int dist[MAX_NODES] = {0};
	list_graph_t *lg;

	printf("UNDIRECTED graph for the minimum distance problem:\n");
	scanf("%u %u", &nodes, &edges);
	lg = lg_create(nodes);

	for (i = 0; i != edges; ++i) {
		scanf("%d %d", &x, &y);
		lg_add_edge(lg, x, y);
		lg_add_edge(lg, y, x);
	}

	for (i = 0; i != nodes; ++i)
		dist[i] = INF;

	min_path(lg, 0, dist);

	printf("Minimum distances to node 0:\n");
	for (i = 0; i != nodes; ++i)
		printf("%u: %d\n", i, dist[i]);
	printf("\n");

	lg_free(lg);
}

static void
test_bipartite(void)
{
	unsigned int i, nodes, edges;
	int color[MAX_NODES] = {0};
	int x, y;
	list_graph_t *lg;

	printf("UNDIRECTED graph for the bipartite graph problem:\n");
	scanf("%u %u", &nodes, &edges);
	lg = lg_create(nodes);
	for (i = 0; i != edges; ++i) {
		scanf("%d %d", &x, &y);
		lg_add_edge(lg, x, y);
		lg_add_edge(lg, y, x);
	}

	if (check_bipartite(lg, color)) {
		printf("Nodes with colour 1:\n");
		for (int i = 0; i < lg->nodes; ++i)
			if (color[i] == 1)
				printf("%d ", i);
		printf("\nNodes with colour 2:\n");
		for (int i = 0; i < lg->nodes; ++i)
			if (color[i] == 2)
				printf("%d ", i);
		printf("\n");
	} else
		printf("The graph is not bipartite\n");

	lg_free(lg);
}

void
test(void)
{
	/* Ex 1 */
	test_connected_comp();

	/* Ex 2 */
	test_topo_sort();

	/* Ex 3 */
	test_min_dist();

	/* Ex 4 */
	test_bipartite();
}

void Problem1(void)
{
	unsigned int num_comps, i, nodes, edges;
	int x, y;
	int visited[MAX_NODES] = {0};
	list_graph_t *lg;
	linked_list_t **comps;

	printf("Problem 1\n");
	scanf("%u %u", &nodes, &edges);
	lg = lg_create(nodes);

	for (i = 0; i != edges; ++i) {
		scanf("%d %d", &x, &y);
		lg_add_edge(lg, x, y);
		lg_add_edge(lg, y, x);
	}

	comps = connected_components(lg, visited, &num_comps);

	printf("Found %d clans:\n", num_comps);
	for (i = 0; i != num_comps; ++i) {
		ll_print_int(comps[i]);
		ll_free(comps + i);
	}
	printf("\n");

	free(comps);
	lg_free(lg);
}

void Problem2_old(void)
{
	#define c_neighbor ((int*)(aux->data))[0]

	unsigned int i, nodes, edges;
	int x, y;
	int start,end;
	int dist[MAX_NODES] = {0};
	list_graph_t *lg;
	linked_list_t *path;

	printf("Problem 2\n");
	scanf("%u %u", &nodes, &edges);
	lg = lg_create(nodes);
	path = ll_create(sizeof(int));

	for (i = 0; i != edges; ++i) {
		scanf("%d %d", &x, &y);
		lg_add_edge(lg, x, y);
		lg_add_edge(lg, y, x);
	}
	scanf("%d %d", &start, &end);
	for (i = 0; i != nodes; ++i)
		dist[i] = INF;

	printf("Shortest path from %d to %d\n",start,end);
	min_path(lg, start, dist);

	ll_add_nth_node(path,0,&end);

	while (start!=end)
		for(ll_node_t *aux=lg->neighbors[end]->head;aux!=NULL;aux=aux->next)
			if(dist[c_neighbor]==dist[end]-1)
			{
				ll_add_nth_node(path,0,aux->data);
				end=c_neighbor;
				break;
			}

	ll_print_int(path);
	printf("\n");

	lg_free(lg);
	ll_free(&path);
	#undef c_neighbor
}

void Problem2(void)
{
	#define c_neighbor ((int*)(aux->data))[0]

	unsigned int i, nodes, edges;
	int x, y;
	int start,end;
	int dist[MAX_NODES] = {0}, origin[MAX_NODES] = {0};
	list_graph_t *lg;
	linked_list_t *path;

	printf("Problem 2\n");
	scanf("%u %u", &nodes, &edges);
	lg = lg_create(nodes);
	path = ll_create(sizeof(int));

	for (i = 0; i != edges; ++i) {
		scanf("%d %d", &x, &y);
		lg_add_edge(lg, x, y);
		lg_add_edge(lg, y, x);
	}
	scanf("%d %d", &start, &end);
	for (i = 0; i != nodes; ++i)
	{
		dist[i] = INF;
		origin[i]=0;
	}

	printf("Shortest path from %d to %d\n",start,end);
	min_path_origin(lg, start, dist,origin);

	ll_add_nth_node(path,0,&end);

	for(;start!=end;end=origin[end])
		ll_add_nth_node(path,0,origin+end);

	ll_print_int(path);
	printf("\n");

	lg_free(lg);
	ll_free(&path);
	#undef c_neighbor
}

void Problem4(void)
{
	unsigned int i, nodes, edges;
	int color[MAX_NODES] = {0};
	int x, y;
	list_graph_t *lg;

	printf("Problem 4\n");
	scanf("%u %u", &nodes, &edges);
	lg = lg_create(nodes);
	for (i = 0; i != edges; ++i) {
		scanf("%d %d", &x, &y);
		lg_add_edge(lg, x, y);
		lg_add_edge(lg, y, x);
	}

	if (check_bipartite(lg, color)) {
		printf("Set 1:\n");
		for (int i = 0; i < lg->nodes; ++i)
			if (color[i] == 1)
				printf("%d ", i);
		printf("\nSet 2:\n");
		for (int i = 0; i < lg->nodes; ++i)
			if (color[i] == 2)
				printf("%d ", i);
		printf("\n");
	} else
		printf("The graph is not bipartite\n");

	printf("\n");

	lg_free(lg);

}

void Problem5(void)
{
	unsigned int i, nodes, edges;
	int visited[MAX_NODES] = {0};
	int x, y;
	list_graph_t *lg;
	linked_list_t *path = ll_create(sizeof(int));

	printf("Problem 5\n");
	scanf("%u %u", &nodes, &edges);
	lg = lg_create(nodes);
	for (i = 0; i != edges; ++i) {
		scanf("%d %d", &x, &y);
		lg_add_edge(lg, x, y);
		lg_add_edge(lg, y, x);
	}

	ll_add_nth_node(path,0,visited);
	visited[0]=1;

	dfs_all_paths(lg,0,visited,path);

	printf("\n");

	lg_free(lg);
	ll_free(&path);


}

int main(void)
{
	Problem1();

	// Problem2_old();
	Problem2();

	Problem4();

	Problem5();

	return 0;
}
