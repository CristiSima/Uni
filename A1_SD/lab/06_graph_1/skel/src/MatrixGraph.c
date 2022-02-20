#include <stdio.h>
#include <stdlib.h>

#include "MatrixGraph.h"
#include "utils.h"

/**
 * Initializeaza graful cu numarul de noduri primit ca parametru si aloca
 * memorie pentru matricea de adiacenta a grafului.
 */
matrix_graph_t*
mg_create(int nodes)
{
	matrix_graph_t *graph=malloc(sizeof(matrix_graph_t));
	DIE(!graph,"graph malloc failed");

	graph->nodes=nodes;
	graph->matrix=malloc(sizeof(int*)*nodes);
	DIE(!graph->matrix,"matrix base malloc failed");

	for(int i=0;i<nodes;i++)
	{
		graph->matrix[i]=calloc(sizeof(int),nodes);
		DIE(!graph->matrix[i],"matrix iteration malloc failed");
	}

	return graph;
}

/* Adauga o muchie intre nodurile sursa si destinatie */
void
mg_add_edge(matrix_graph_t* graph, int src, int dest)
{
	DIE(!graph,"got no graph");
	DIE(graph->nodes<=src ,"src  out of range");
	DIE(graph->nodes<=dest,"dest out of range");

	graph->matrix[src ][dest]=1;
	graph->matrix[dest][src ]=1;
}

/* Returneaza 1 daca exista muchie intre cele doua noduri, 0 in caz contrar */
int
mg_has_edge(matrix_graph_t* graph, int src, int dest)
{
	DIE(!graph,"got no graph");
	DIE(graph->nodes<=src ,"src  out of range");
	DIE(graph->nodes<=dest,"dest out of range");

	return graph->matrix[src][dest];
}

/* Elimina muchia dintre nodurile sursa si destinatie */
void
mg_remove_edge(matrix_graph_t* graph, int src, int dest)
{
	DIE(!graph,"got no graph");
	DIE(graph->nodes<=src ,"src  out of range");
	DIE(graph->nodes<=dest,"dest out of range");

	graph->matrix[src ][dest]=0;
	graph->matrix[dest][src ]=0;
}

/* Elibereaza memoria folosita de matricea de adiacenta a grafului */
void
mg_free(matrix_graph_t* graph)
{
	DIE(!graph,"got no graph");

	for(int i=0;i<graph->nodes;i++)
		free(graph->matrix[i]);
		
	free(graph->matrix);
	free(graph);
}
