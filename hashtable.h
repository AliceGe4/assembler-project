#ifndef _HASHTABLE_H
#define _HASHTABLE_H
/* We use void* to enable the hashtable to change type.*/
typedef struct {
	/*A key of the item in the hash*/
	void* key;
	/*The value of the item, what it contains.*/
	void* value;

}HASHTABLE_ITEM;

/*HASHTABLE is a collection of HASHTABLE_ITEM*/

typedef struct {
	HASHTABLE_ITEM* ht;
	int size_ht;
}HASHTABLE;
/**
* insert_item_hash allows us to save the items into the hash table
* @param hash a pointer to the Hashtable
* @param key a pointer to the key of the hash
* @param value the value of the current hash
* @return returns wether or not it was able to insert the item into the hash.
*/
int insert_item_hash(HASHTABLE* hash,void* key,void* value, int(*compare)(void*, void*));

/**
* allows us to pull the detail/string that is associated with current key in the hash table.
* @param hash a pointer to the Hashtable
* @param key a pointer to the key of the hash
* @return returns the value of the hash
*/
void* pull_item_hash(HASHTABLE* hash,void* key, int(*compare)(void*, void*));

/**
*iniiates the hashtable
* @param hash a pointer to the Hashtable
*/
void initiate_hash(HASHTABLE* hash);

/**
* Frees up memory for hash_table
* @param hash a pointer to the Hashtable
*/
void free_hashtable(HASHTABLE* hash, void(*free_key)(void*), void(*free_value)(void*));

#endif
