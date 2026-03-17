#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "hashtable.h"
#include "assembler.h"

int insert_item_hash(HASHTABLE* hash, void* key, void* value, int(*compare)(void*, void*)) {
    /* Before allocating memory, is_exsists attempts to check if the key is already in the hash.*/
    void* is_exsists = pull_item_hash(hash, key, compare);
    if (is_exsists == NULL) {
        /*allocates a new area in the hash*/
        hash->ht = (HASHTABLE_ITEM*)realloc(hash->ht, (hash->size_ht + 1) * sizeof(HASHTABLE_ITEM));
        hash->ht[hash->size_ht].key = key;
        hash->ht[hash->size_ht].value = value;
        /*increase the function size by one.*/
        hash->size_ht++;
        return 1;
    } 
    return 0;
}

void* pull_item_hash(HASHTABLE* hash,void* key, int(*compare)(void*, void*)) {
    int i;
    /*This loops runs on all elements of the hashtable_item, attempting to pull a key of a hash. If it finds it, it equals to 0 and returns it.*/
    for (i = 0; i < hash->size_ht; i++) {
        if (compare(key, hash->ht[i].key) == 0)
            return hash->ht[i].value;
    }
    return NULL;
}

void initiate_hash(HASHTABLE* hash) {
    hash->ht = NULL;
    hash->size_ht = 0;
}

void free_hashtable(HASHTABLE* hash, void(*free_key)(void*), void(*free_value)(void*)) {
    int i;
    for (i = 0; i < hash->size_ht; i++) {
        if (free_key != NULL) {
            free_key(hash->ht[i].key);
        }
        if (free_value != NULL) {
            free_value(hash->ht[i].value);
        }
    }
    free(hash->ht);
}
