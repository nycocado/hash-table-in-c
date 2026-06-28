#pragma once

#include "list.h"
#include <stdbool.h>

#define DEFAULT_SIZE 101

/**
 * @brief A hash table maps keys to values.
 *
 * The keys and values can be of any type, but the hash table itself is
 * homogeneous.
 */
typedef struct _HashTable* HashTable;

/**
 * @brief Creates a new hash table with the given size, hash function and key
 * equality function.
 *
 * The hash function should return a number between 0 and size - 1. The key
 * equality function should return true if the two keys are equal, false
 * otherwise.
 *
 * @param size The number of buckets in the hash table.
 * @param hash The hash function.
 * @param key_equal The key equality function.
 * @return HashTable The new hash table.
 */
HashTable hash_table_create(
    int size,
    int (*hash)(void*, int),
    bool (*key_equal)(void*, void*)
);

/**
 * @brief Destroys the hash table, freeing all the memory used by it.
 *
 * If key_destroy or value_destroy are not NULL, they will be called for each
 * key and value in the hash table.
 *
 * @param htable The hash table to destroy.
 * @param key_destroy The function to free the keys of the hash table.
 * @param value_destroy The function to free the values of the hash table.
 */
void hash_table_destroy(
    HashTable htable,
    void (*key_destroy)(void*),
    void (*value_destroy)(void*)
);

/**
 * @brief Returns true iff the hash table is empty.
 *
 * @param htable The hash table.
 * @return true iff the hash table is empty.
 */
bool hash_table_is_empty(HashTable htable);

/**
 * @brief Returns the number of elements in the hash table.
 *
 * @param htable The hash table.
 * @return int The number of elements in the hash table.
 */
int hash_table_size(HashTable htable);

/**
 * @brief Inserts a new key-value pair in the hash table.
 *
 * If the key already exists, the value will be updated. Returns NULL if the
 * key is new, or the old value if the key already exists.
 *
 * @param htable The hash table.
 * @param key The key to insert.
 * @param value The value to insert.
 * @return void* NULL if the key is new, or the old value if the key already
 * exists.
 */
void* hash_table_insert(HashTable htable, void* key, void* value);

/**
 * @brief Removes the key-value pair with the given key from the hash table
 * and returns the value.
 *
 * @param htable The hash table.
 * @param key The key to remove.
 * @return void* The value associated with the removed key.
 */
void* hash_table_remove(HashTable htable, void* key);

/**
 * @brief Returns the value associated with the given key, or NULL if the key
 * is not in the hash table.
 *
 * @param htable The hash table.
 * @param key The key to search for.
 * @return void* The value associated with the given key, or NULL if the key
 * is not in the hash table.
 */
void* hash_table_get(HashTable htable, void* key);

/**
 * @brief Returns a linked list with all the keys in the hash table.
 *
 * @param htable The hash table.
 * @return List A linked list with all the keys in the hash table.
 */
List hash_table_keys(HashTable htable);

/**
 * @brief Returns a linked list with all the values in the hash table.
 *
 * @param htable The hash table.
 * @return List A linked list with all the values in the hash table.
 */
List hash_table_values(HashTable htable);

/**
 * @brief Rehashes the hash table to a new size.
 *
 * If key_destroy or value_destroy are not NULL, they will be called for each
 * key and value in the hash table. If new_size is less than or equal to 0,
 * no rehashing will be done.
 *
 * @param htable The hash table.
 * @param new_size The new size of the hash table.
 * @param key_destroy The function to free the keys of the hash table.
 * @param value_destroy The function to free the values of the hash table.
 * @return HashTable The rehashed hash table.
 */
HashTable hash_table_rehash(
    HashTable htable,
    int new_size,
    void (*key_destroy)(void*),
    void (*value_destroy)(void*)
);
