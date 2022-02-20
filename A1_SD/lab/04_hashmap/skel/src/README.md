# Hashtable.c

## ht_create
```c
hashtable_t *ht_create(unsigned int hmax, unsigned int (*hash_function)(void*), int (*compare_function)(void*, void*));
```
allocates memory

## ht_put
```c
void ht_put(hashtable_t *ht, void *key, unsigned int key_size, void *value, unsigned int value_size)
```
selects the apropriate list and searches it for the key<BR>
if the key is found the memory is copied over value<BR>
else a new info object is created and appended to the list

## ht_get
```c
void *ht_get(hashtable_t *ht, void *key)
```
selects the apropriate list and searches it for the key

## ht_has_key
```c
int ht_has_key(hashtable_t *ht, void *key)
```
selects the apropriate list and searches it for the key

## ht_remove_entry
```c
void ht_remove_entry(hashtable_t *ht, void *key)
```
selects the apropriate list and searches it for the key

## ht_free
```c
void ht_free(hashtable_t *ht)
```
iterates over all the buckets and frees the list

<BR>
<BR>

# Banca.c

## client_create
```c
client_t *client_create(char *name,unsigned int pin,int balance)
```
creates object

## client_free
```c
void client_free(client_t *client)
```
frees memory

## transaction_create
```c
transaction_t *transaction_create(char *place, ushort day, ushort month, ushort year, int ammount)
```
creates object

## transaction_print
```c
void transaction_print(transaction_t *transaction)
```
prints transaction info ``"\t- %d on %02d.%02d.%04d\n"``

## transaction_write_date
```c
void transaction_write_date(transaction_t *transaction, char *name)
```
writes the date in ``date``

## client_show_transactions
```c
void client_show_transactions(client_t *client)
```
prints all the transactions using [`transaction_print`](#transaction_print) by iterating over the transaction list

## ht_free_Wfree
```c
void ht_free_Wfree(hashtable_t *ht, free_function_t value_free)
```
just like [`ht_free`](#ht_free) but uses a specified function to free ``info->value``

## add_to_ht_list
```c
void add_to_ht_list(hashtable_t *ht,char *key, uint key_size, void *value, uint value_size)
```
append an element to the end of a list obtained from a ht<BR>
creating the list if needed

## get_transactions_on
```c
void get_transactions_on(hashtable_t *ht, char *date)
```
prints all the transactions using [`transaction_print`](#transaction_print) by iterating over the transaction list from a ht

<BR>

## d
using a ht where the key is obtained from [`transaction_write_date`](#transaction_write_date) and value contains a list with transactions

<BR>
<BR>

# Resize.c

## ht_resize
```c
hashtable_t *ht_resize(hashtable_t *ht,unsigned int new_hmax)
```
creates a new ht and adds all the elements in the old ht<BR>
[`ht_put`](#ht_put) isn't used bc the key isn't present so a new node will be added to he list<BR>
isn't usable as is without providing `key_size` & `value_size` needed to completly copy `struct info` in order to avoid a double free

## ht_resize_replace
```c
void ht_resize_replace(hashtable_t *ht,unsigned int new_hmax)
```
creates a new list vector and adds all the elements in the old ht while removeing them from the old list<BR>
that replaces the old one and `->hmax is overwriten`<BR>
the result is a new and fuctional ht at the same place in memory<BR>
[`ht_put`](#ht_put) isn't used bc the key isn't present so a new node will be added to he list<BR>

<BR>
<BR>

# Leetcode

## isAnagram
```c
bool isAnagram(char * s, char * t)
```
counts all the leters in eatch string and compares the result

## twoSum
```c
int* twoSum(int* nums, int numsSize, int target, int* returnSize){
```
checks all combinations
