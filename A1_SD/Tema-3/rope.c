#include "./rope.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define EMPTY ""

char *strdup(const char *s);
char *strndup(const char *s, size_t size);

void *memdup(const void *src, size_t size)
{
    void *dest = malloc(size);

    memcpy(dest, src, size);

    return dest;
}

static void __RopeNode_free(RopeNode *node)
{
	if(!node)
		return;

	__RopeNode_free(node->left);
	__RopeNode_free(node->right);

	free((void*)node->str);
	free(node);
}

void RopeTree_free(RopeTree *rp)
{
	__RopeNode_free(rp->root);
	free(rp);
}

RopeNode* makeRopeNode(const char* str) {
    RopeNode *rn = malloc(sizeof(*rn));

    rn->left = rn->right = NULL;
    rn->str = str;
    rn->weight = strlen(str);

    return rn;
}

RopeTree* makeRopeTree(RopeNode* root) {
	RopeTree *rt = malloc(sizeof(RopeTree));

	rt->root = root;

	return rt;
}


void printRopeNode(RopeNode* rn) {
    if (!rn)
        return;

    if (!(rn->left) && !(rn->right)) {
        printf("%s", rn->str);
        return;
    }

    printRopeNode(rn->left);
    printRopeNode(rn->right);
}

void printRopeTree(RopeTree* rt) {
    if (rt && rt->root) {
        printRopeNode(rt->root);
        printf("%s", "\n");
    }
}

void debugRopeNode(RopeNode* rn, int indent) {
    if (!rn)
        return;

    for (int i = 0; i < indent; ++i)
        printf("%s", " ");

    if (!strcmp(rn->str, EMPTY))
        printf("# %d\n", rn->weight);
    else
        printf("%s %d\n", rn->str, rn->weight);

    debugRopeNode(rn->left, indent+2);
    debugRopeNode(rn->right, indent+2);
}



int getTotalWeight(RopeNode* rt) {
    if (!rt)
        return 0;

    return rt->weight + getTotalWeight(rt->right);
}


RopeTree* concat(RopeTree* rt1, RopeTree* rt2) {
	RopeNode *rn = makeRopeNode(strdup(EMPTY));

	rn->weight = getTotalWeight(rt1->root);
	rn->left = rt1->root;
	rn->right = rt2->root;

	return makeRopeTree(rn);
}



char indexRope(RopeTree* rt, int idx) {
    RopeNode *rn = rt->root;
    if (idx > getTotalWeight(rn))
        return 0;

    while (1)
    {
        if (rn->weight <= idx)
        {
            idx -= rn->weight;
            rn = rn->right;
        }
        else if (rn->str[0] == 0)
            rn = rn->left;
        else
            return rn->str[idx];
    }
}

void __search_rec(RopeNode *rn, int start, int end, char *str)
{
	if(rn->str[0] == 0)
	{
		if(rn->left && start <= rn->weight)
			__search_rec(rn->left, start, end, str);

		if (rn->right && rn->weight < end)
			__search_rec(rn->right, start-rn->weight, end-rn->weight, str);
	}
	else
	{
		int i = 0;
		if(start > 0)
			i = start;

		start *= -1;

		if(rn->weight < end)
			end = rn->weight;

		for(; i < end; i++)
			str[start+i] = rn->str[i];
		str[start+i] = 0;
	}
}

char* search(RopeTree* rt, int start, int end) {
	char *str = malloc(end-start+1);

	__search_rec(rt->root, start, end, str);

	return str;
}

RopeNode *deep_copy_node(RopeNode *rn)
{
    if (!rn)
        return NULL;

    RopeNode *cp = makeRopeNode(strdup(rn->str));

    cp->left = deep_copy_node(rn->left);
    cp->right = deep_copy_node(rn->right);
    cp->weight = rn->weight;

    return cp;
}

SplitPair split(RopeTree* rt, int idx) {
    RopeNode *rn = rt->root;
    RopeTree *fake_tree;
    SplitPair pair;

    if (idx == 0)
    {
        pair.left = makeRopeNode(strdup(EMPTY));
        pair.right = deep_copy_node(rn);

        return pair;
    }
    else if (idx == getTotalWeight(rn))
    {
        pair.left = deep_copy_node(rn);
        pair.right = makeRopeNode(strdup(EMPTY));

        return pair;
    }
    if (!rn->right && !rn->left)
    {
		pair.left = makeRopeNode(strndup(rn->str, idx));

		if(idx > rn->weight)
			idx = rn->weight;

		pair.right = makeRopeNode(strdup(rn->str + idx));

        return pair;
    }
    else if (idx - 1 >= rn->weight)
    {
        fake_tree = makeRopeTree(rn->right);
        pair = split(fake_tree, idx - rn->weight);
        free(fake_tree);

        rn = memdup(rn, sizeof(RopeNode));
		rn->str = strdup(rn->str);
        rn->left = deep_copy_node(rn->left);
        rn->right = pair.left;

        pair.left = rn;
    }
    else
    {
        fake_tree = makeRopeTree(rn->left);
        pair = split(fake_tree, idx);
        free(fake_tree);

        rn = memdup(rn, sizeof(RopeNode));
		rn->str = strdup(rn->str);
        rn->right = deep_copy_node(rn->right);

        if (pair.right && rn->right)
        {
            RopeTree *fake_tree1 = makeRopeTree(pair.right);
            RopeTree *fake_tree2 = makeRopeTree(rn->right);
            fake_tree = concat(fake_tree1, fake_tree2);

            pair.right = fake_tree->root;
			pair.right->weight = getTotalWeight(pair.right->left);

            free(fake_tree);
            free(fake_tree1);
            free(fake_tree2);
        }
        else if (rn->right)
            pair.right = rn->right;

        rn->left = pair.left;
        pair.left = rn;

        rn->weight = getTotalWeight(rn->left);
        rn->right = NULL;
    }

    return pair;
}

RopeTree* insert(RopeTree* rt, int idx, const char* str) {
	SplitPair pair = split(rt, idx);
	RopeTree *fake_tree1 = makeRopeTree(pair.left);
	RopeTree *fake_tree2 = makeRopeTree(pair.right);
	RopeTree *fake_tree = makeRopeTree(makeRopeNode(str));


	rt = concat(fake_tree1, fake_tree);
	free(fake_tree1);
	free(fake_tree);

	fake_tree = rt;
	rt = concat(fake_tree, fake_tree2);
	free(fake_tree);
	free(fake_tree2);

	return rt;
}

RopeTree* delete(RopeTree* rt, int start, int len) {
	SplitPair pair1 = split(rt, start);
	RopeTree *fake_tree = makeRopeTree(pair1.right);

	SplitPair pair2 = split(fake_tree, len);
	RopeTree_free(fake_tree);
	__RopeNode_free(pair2.left);

	RopeTree *fake_tree1 = makeRopeTree(pair1.left);
	RopeTree *fake_tree2 = makeRopeTree(pair2.right);

	fake_tree = concat(fake_tree1, fake_tree2);

	free(fake_tree1);
	free(fake_tree2);

	return fake_tree;
}
