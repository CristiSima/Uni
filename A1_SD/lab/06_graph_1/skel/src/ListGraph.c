#include <stdio.h>
#include <stdlib.h>

#include "ListGraph.h"
#include "utils.h"

list_graph_t*
lg_create(int nodes)
{
	list_graph_t *graph=malloc(sizeof(list_graph_t));
	DIE(!graph,"graph malloc failed");

	graph->nodes=nodes;
	graph->neighbors=malloc(sizeof(linked_list_t*)*nodes);
	DIE(!graph->neighbors,"neighbors vector malloc failed");

	for(int i=0;i<nodes;i++)
		graph->neighbors[i]=ll_create(sizeof(int));

	return graph;
}

void
lg_add_edge(list_graph_t* graph, int src, int dest)
{
	DIE(!graph,"got no graph");
	DIE(graph->nodes<=src ,"src  out of range");
	DIE(graph->nodes<=dest,"dest out of range");

	ll_add_nth_node(graph->neighbors[src] ,0,&dest);
	ll_add_nth_node(graph->neighbors[dest],0,&src );
}

int
lg_has_edge(list_graph_t* graph, int src, int dest)
{
	DIE(!graph,"got no graph");
	DIE(graph->nodes<=src ,"src  out of range");
	DIE(graph->nodes<=dest,"dest out of range");

	ll_node_t *aux=graph->neighbors[src]->head;
	for(;aux!=NULL;aux=aux->next)
		if(((int*)(aux->data))[0]==dest)
			return 1;
	return 0;
}

linked_list_t*
lg_get_neighbours(list_graph_t* graph, int node)
{
	DIE(!graph,"got no graph");
	DIE(graph->nodes<=node,"node out of range");

	return graph->neighbors[node];
}

void
lg_remove_edge(list_graph_t* graph, int src, int dest)
{
	DIE(!graph,"got no graph");
	DIE(graph->nodes<=src ,"src  out of range");
	DIE(graph->nodes<=dest,"dest out of range");

	ll_node_t *node,*aux;

	aux=graph->neighbors[src]->head;
	for(int i=0;aux!=NULL;aux=aux->next,i++)
		if(((int*)(aux->data))[0]==dest)
		{
			node=ll_remove_nth_node(graph->neighbors[src],i);
			free(node->data);
			free(node);
			break;
		}

	aux=graph->neighbors[dest]->head;
	for(int i=0;aux!=NULL;aux=aux->next,i++)
		if(((int*)(aux->data))[0]==src )
		{
			node=ll_remove_nth_node(graph->neighbors[dest],i);
			free(node->data);
			free(node);
			break;
		}
}

void
lg_free(list_graph_t* graph)
{
	DIE(!graph,"got no graph");

	for(int i=0;i<graph->nodes;i++)
		ll_free(graph->neighbors+i);
		
	free(graph->neighbors);
	free(graph);
}
