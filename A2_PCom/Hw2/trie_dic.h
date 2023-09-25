#ifndef _TRIE_DIC_H
#define _TRIE_DIC_H 1
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// #ifndef memdup
void* memdup(const void* mem, size_t size);
// #endif memdup


struct trie_dic_node
{
    void *data;
    struct trie_dic_node* next[2];
};

typedef struct trie_dic_node *trie_dic;

trie_dic trie_dic_create();

void *trie_dic_get(trie_dic dic, uint8_t *key, size_t key_size);

// wrapper for defineing type specific function for geting
#define trie_dic_get_instance(name, key_len, val_type) val_type *name(trie_dic dic, void *key) \
                                                            {return (void*)trie_dic_get(dic, key, key_len);}

void trie_dic_set(trie_dic dic, uint8_t *key, size_t key_size, void *data);

// wrapper for defineing type specific function for seting
#define trie_dic_set_instance(name, key_len, val_type) void name(trie_dic dic, void *key, void *data) \
                                                            {trie_dic_set(dic, key, key_len, data);}


trie_dic _trie_dic_free(trie_dic dic, void free_func(void*));

// wrapper for nice code
#define trie_dic_free(dic, free_func)   dic=_trie_dic_free(dic, free_func)

#endif // _TRIE_DIC_H
