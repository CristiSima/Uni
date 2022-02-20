#include <stdio.h>
#include <stdlib.h>

#include "ListGraph.h"
#include "MatrixGraph.h"
#include "Queue.h"

#define MAX_NODES 10

void
print_matrix_graph(matrix_graph_t* mg)
{
	printf("%d\n",mg->nodes);

	for(int i=0;i<mg->nodes;i++)
	{
		for(int j=0;j<mg->nodes;j++)
			printf("%d ",mg_has_edge(mg,i,j));

		printf("\n");
	}
}

void
print_list_graph(list_graph_t* lg)
{
	printf("%d\n",lg->nodes);

	for(int i=0;i<lg->nodes;i++)
	{
		printf("%i: ",i);
		ll_print_int(lg->neighbors[i]);
	}
}

void
dfs_list_graph(list_graph_t* lg,
		int node,
		int* visited,
		int* st_node,
		int* st_iter)
{
	#define get_int(node) (((int*)((node)->data))[0])

	int depth=0;
	int i;
	ll_node_t *aux;

	for(int i=0;i<lg->nodes;i++)
		visited[i]=0;

	st_node[0]=node;
	st_iter[0]=0;
	visited[node]=1;

	printf("%d ",node);

	while(depth!=-1)
	{
		aux=lg->neighbors[st_node[depth]]->head;

		for(i=0;i<st_iter[depth] && aux!=NULL;i++)
			aux=aux->next;

		for(;aux!=NULL;i++,aux=aux->next)
			if(!visited[get_int(aux)] &&
				lg_has_edge(lg,st_node[depth],get_int(aux)))
			{
				printf("%d ",get_int(aux));

				visited[get_int(aux)]=1;
				st_iter[depth]=i;

				depth++;
				st_node[depth]=get_int(aux);
				st_iter[depth]=0;

				break;
			}

		if(aux==NULL)
			depth--;
	}
	printf("\n");

	#undef get_int
}

void
dfs_matrix_graph(matrix_graph_t* mg,
		 int node,
		 int* visited,
		 int* st_node,
		 int* st_iter)
{
	int depth=0;
	int i;

	for(int i=0;i<mg->nodes;i++)
		visited[i]=0;

	st_node[0]=node;
	st_iter[0]=0;
	visited[node]=1;

	printf("%d ",node);

	while(depth!=-1)
	{
		for(i=st_iter[depth];i<mg->nodes;i++)
			if(!visited[i] && mg_has_edge(mg,st_node[depth],i))
			{
				printf("%d ",i);

				visited[i]=1;
				st_iter[depth]=i;

				depth++;
				st_iter[depth]=0;
				st_node[depth]=i;

				i=0;
				break;
			}

		if(i)
			depth--;
	}
	printf("\n");
}

void
bfs_list_graph(list_graph_t* lg, int node, int* color, int* parent)
{
	#define get_int(node) (((int*)((node)->data))[0])

	queue_t *queue=q_create(sizeof(int),lg->nodes);
	ll_node_t *aux;
	int cur;

	for(int i=0;i<lg->nodes;i++)
	{
		color[i]=-1;
		parent[i]=-1;
	}

	color[node]=0;
	q_enqueue(queue,&node);

	printf("%d ",node);

	while(!q_is_empty(queue))
	{
		cur=((int*)q_front(queue))[0];
		aux=lg->neighbors[cur]->head;

		for(;aux!=NULL;aux=aux->next)
			if(color[get_int(aux)]==-1)
			{
				printf("%d ", get_int(aux));

				color[get_int(aux)]=color[cur]+1;
				parent[get_int(aux)]=cur;

				q_enqueue(queue,aux->data);
			}

		q_dequeue(queue);
	}
	printf("\n");

	q_free(queue);
	#undef get_int
}

void
bfs_matrix_graph(matrix_graph_t* mg, int node, int* color, int* parent)
{
	queue_t *queue=q_create(sizeof(int),mg->nodes);
	int cur;

	for(int i=0;i<mg->nodes;i++)
	{
		color[i]=-1;
		parent[i]=-1;
	}

	color[node]=0;
	q_enqueue(queue,&node);

	printf("%d ",node);

	while(!q_is_empty(queue))
	{
		cur=((int*)q_front(queue))[0];
		
		for(int i=0;i<mg->nodes;i++)
			if(mg_has_edge(mg,cur,i) && color[i]==-1)
			{
				printf("%d ", i);

				color[i]=color[cur]+1;
				parent[i]=cur;

				q_enqueue(queue,&i);
			}

		q_dequeue(queue);
	}
	printf("\n");
	q_free(queue);
	#undef get_int
}

void
floyd_warshall(matrix_graph_t* mg)
{
	for(int k=0;k<mg->nodes;k++)
		for(int i=0;i<mg->nodes;i++) if(mg_has_edge(mg,i,k))
			for(int j=0;j<mg->nodes;j++) if(mg_has_edge(mg,k,j))
				if(mg->matrix[i][j]==0 ||
					mg->matrix[i][j]>mg->matrix[i][k]+mg->matrix[k][j])
				{
					mg->matrix[i][j]=mg->matrix[i][k]+mg->matrix[k][j];
					mg->matrix[j][i]=mg->matrix[i][k]+mg->matrix[k][j];
				}

}

int
main()
{
	int nodes, edges;
	int x[MAX_NODES], y[MAX_NODES];
	int visited[MAX_NODES], t_dest[MAX_NODES], t_fin[MAX_NODES];
	list_graph_t* lg = lg_create(MAX_NODES);
	matrix_graph_t* mg = mg_create(MAX_NODES);

	int numbers[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	printf("-------------------------------- Test ListGraph "
		"--------------------------------\n");
	/* Test add_edge_list_graph has_edge_list_graph */

	lg_add_edge(lg, numbers[0], numbers[1]);
	lg_add_edge(lg, numbers[3], numbers[2]);

	printf("#1:\nOutput: %d\nOutput asteptat: 1\n",
		lg_has_edge(lg, numbers[0], numbers[1]));

	printf("#2:\nOutput: %d\nOutput asteptat: 1\n",
		lg_has_edge(lg, numbers[3], numbers[2]));

	printf("#3:\nOutput: %d\nOutput asteptat: 0\n",
		lg_has_edge(lg, numbers[2], numbers[4]));

	/* Test remove_edge_list_graph */
	lg_remove_edge(lg, numbers[0], numbers[1]);
	printf("#4:\nOutput: %d\nOutput asteptat: 0\n",
		lg_has_edge(lg, numbers[0], numbers[1]));

	/* Test get_neighbours_list_graph */
	lg_add_edge(lg, numbers[0], numbers[1]);
	lg_add_edge(lg, numbers[0], numbers[2]);
	lg_add_edge(lg, numbers[0], numbers[3]);
	lg_add_edge(lg, numbers[0], numbers[4]);

	linked_list_t* l = lg_get_neighbours(lg, numbers[0]);

	printf("#5:\n");
	printf("Output: ");
	ll_print_int(l);
	printf("Output asteptat: 1 2 3 4\n");

	printf("-------------------------------- Test MatrixGraph "
		"--------------------------------\n");
	/* test add edge_matrix_graph has_edge_matrix_graph */
	mg_add_edge(mg, numbers[0], numbers[1]);
	mg_add_edge(mg, numbers[3], numbers[2]);

	printf("#1:\nOutput: %d\nOutput asteptat: 1\n",
		mg_has_edge(mg, numbers[0], numbers[1]));

	printf("#2:\nOutput: %d\nOutput asteptat: 1\n",
		mg_has_edge(mg, numbers[3], numbers[2]));

	printf("#3:\nOutput: %d\nOutput asteptat: 0\n",
		mg_has_edge(mg, numbers[2], numbers[4]));

	/* test remove_edge_matrix_graph */
	mg_remove_edge(mg, numbers[0], numbers[1]);
	printf("#4:\nOutput: %d\nOutput asteptat: 0\n",
		mg_has_edge(mg, numbers[0], numbers[1]));


	mg_add_edge(mg,1,2);
	mg_add_edge(mg,0,2);
	mg_add_edge(mg,0,6);

	print_matrix_graph(mg);
	dfs_matrix_graph(mg,0,visited,t_dest,t_fin);
	bfs_matrix_graph(mg,0,t_dest,t_fin);

	floyd_warshall(mg);
	print_matrix_graph(mg);

	print_list_graph(lg);
	dfs_list_graph(lg,2,visited,t_dest,t_fin);
	bfs_list_graph(lg,2,t_dest,t_fin);

	lg_free(lg);
	mg_free(mg);
	return 0;
}
