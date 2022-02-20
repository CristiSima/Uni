/**
 * SD, 2020
 *
 * Lab #8, Binary Tree
 *
 * Task #bonus - b_tree_free with memory of O(1)
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

void b_tree_nice_free(b_tree_t* b_tree, void (*free_data)(void*))
{
	b_node_t *root=b_tree->root;
	b_node_t *aux;
	b_node_t *node;

	for(aux=root;aux->left!=NULL;aux=aux->left);

	while(root!=NULL)
	{
		if(root->right!=NULL)
		{
			aux->left=root->right;
			for(;aux->left!=NULL;aux=aux->left);
		}
		
		node=root;

		root=root->left;

		free_data(node->data);
		free(node);
	}

	free(b_tree);
}

int main(void)
{
    /* TODO */
    b_tree_t *binary_tree;

    binary_tree = b_tree_create(sizeof(int));

    read_tree(binary_tree);

	b_tree_nice_free(binary_tree,free);

	return 0;
}
