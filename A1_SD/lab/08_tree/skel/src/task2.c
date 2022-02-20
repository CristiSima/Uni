/**
 * SD, 2020
 *
 * Lab #8, Binary Tree
 *
 * Task #2 - all paths to the leafs with max sum
 */

#include "binary_tree.h"

void read_tree(b_tree_t *b_tree)
{
	int i, N, data;

	scanf("%d\n", &N);

	for (i = 0; i < N; ++i) {
		scanf("%d ", &data);
		b_tree_insert(b_tree, &data);
	}
}

void print_data(void *data)
{
	printf("%d ", *(int *) data);
}

static void __calc_max_path(b_node_t* b_node)
{
	#define data_int(node) ((int*)node->data)[0]
	if(!b_node)
		return;
	__calc_max_path(b_node->left);
	__calc_max_path(b_node->right);

	if(b_node->left==NULL)
		return;

	if(b_node->right==NULL)
		data_int(b_node)+=data_int(b_node->left);
	else if(data_int(b_node->left)>data_int(b_node->right))
		data_int(b_node)+=data_int(b_node->left);
	else
		data_int(b_node)+=data_int(b_node->right);

	#undef data_int
}
void calc_max_path(b_tree_t* b_tree)
{
	__calc_max_path(b_tree->root);
}

static void __print_stack(int *stack, int depth)
{
	stack[depth]=0;

	for(int i=0;i<depth;i++)
		printf("%d ", stack[i]-stack[i+1]);

	printf("\n");
}
static void __print_max_path(b_node_t* b_node, int *stack, int depth)
{
	#define data_int(node) ((int*)node->data)[0]
	if(!b_node)
	{
		__print_stack(stack,depth);
		return;
	}
	stack[depth]=data_int(b_node);

	if(b_node->left==NULL)
		__print_max_path(NULL		  ,stack,depth+1);
	else if(b_node->right==NULL)
		__print_max_path(b_node->left ,stack,depth+1);
	else if(data_int(b_node->left)==data_int(b_node->right))
	{
		__print_max_path(b_node->left ,stack,depth+1);
		__print_max_path(b_node->right,stack,depth+1);
	}
	else if(data_int(b_node->left)>data_int(b_node->right))
		__print_max_path(b_node->left ,stack,depth+1);
	else
		__print_max_path(b_node->right,stack,depth+1);


	#undef data_int
}
void print_max_path(b_tree_t* b_tree)
{
	int *stack=malloc(sizeof(int)*(b_tree_height(b_tree)+1));

	__print_max_path(b_tree->root,stack,0);

	free(stack);
}

int main(void)
{
    b_tree_t *binary_tree;

    binary_tree = b_tree_create(sizeof(int));

    read_tree(binary_tree);

	calc_max_path(binary_tree);
	print_max_path(binary_tree);

	b_tree_free(binary_tree,free);

	return 0;
}
