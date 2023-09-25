#include "trie_dic.h"

#ifndef memdup
void* memdup(const void* mem, size_t size)
{
    void* out = malloc(size);

    if(out != NULL)
        memcpy(out, mem, size);

    return out;
}
#endif // memdup

// struct trie_dic_node
// {
//     void *data;
//     struct trie_dic_node* next[2];
// };


trie_dic trie_dic_create()
{
    struct trie_dic_node *root=calloc(1, sizeof(struct trie_dic_node));

    //root->next[0]=calloc(1, sizeof(struct trie_dic_node));
    //root->next[1]=calloc(1, sizeof(struct trie_dic_node));

    return root;
}

void *trie_dic_get(trie_dic dic, uint8_t *key, size_t key_size)
{
    struct trie_dic_node *node=dic;
    for(size_t  i=0;i<key_size;i++)
        for(uint8_t j=0;j<8;j++)
        {
            node=node->next[(key[i]>>j)&1];
            if(node==NULL)
                return NULL;
        }
    return node->data;
}

// #define trie_dic_get_instance(name, key_len, val_type) val_type *name(trie_dic dic, void *key) \
//                                                             {return (void*)trie_dic_get(dic, key, key_len);}

void trie_dic_set(trie_dic dic, uint8_t *key, size_t key_size, void *data)
{
    struct trie_dic_node *node=dic;
    for(size_t  i=0;i<key_size;i++)
        for(uint8_t j=0;j<8;j++)
        {
            if(node->next[(key[i]>>j)&1]==NULL)
                node->next[(key[i]>>j)&1]=calloc(1, sizeof(struct trie_dic_node));

            node=node->next[(key[i]>>j)&1];
        }

    node->data=data;
}

// #define trie_dic_set_instance(name, key_len, val_type) void name(trie_dic dic, void *key, void *data) \
//                                                             {trie_dic_set(dic, key, key_len, data);}


trie_dic _trie_dic_free(trie_dic dic, void free_func(void*))
{
    struct trie_dic_node *queue_end=dic;
    struct trie_dic_node *root=dic;
    struct trie_dic_node *temp;

    for(;queue_end->next[0]; queue_end=queue_end->next[0]);

    while(root)
    {
        if(root->data)
            free_func(root->data);

        queue_end->next[0]=root->next[1];
        for(;queue_end->next[0]; queue_end=queue_end->next[0]);

        temp=root;
        root=root->next[0];
        free(temp);
    }

    return NULL;
}
// #define trie_dic_free(dic, free_func)   dic=_trie_dic_free(dic, free_func)
