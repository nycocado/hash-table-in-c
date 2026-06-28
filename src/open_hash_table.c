#include "hash_table.h"
#include "list.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Data type definitions
typedef struct _Item* Item;
typedef int (*Hash)(void*, int);
typedef bool (*Key_Equal)(void*, void*);

// Data structures
struct _HashTable
{
        int size;
        int arr;
        Hash hash;
        Key_Equal key_equal;
        List* list;
};

struct _Item
{
        void* key;
        void* value;
};

// Helper functions
int default_hash(
    void* key,
    int n
) // Default hash function - O(n) - n is character count
{
    int result = 0; // Initializes result
    int a =
        127; // 127 is a prime number, recommended for this kind of operation
    for (int i = 0; ((char*)key)[i] != '\0'; i++) // While there are characters
    {
        result = (result * a + ((char*)key)[i]); // Performs the math operation
    }
    return result % n; // Returns the index
}

bool default_key_equal(
    void* key1,
    void* key2
) // Default key comparison function - O(1)
{
    return strcmp((char*)key1, (char*)key2) == 0;
}

// HashTable functions
HashTable hash_table_create(
    int size,
    int (*hash)(void*, int),
    bool (*key_equal)(void*, void*)
) // O(n) - worst case
{
    HashTable htable = malloc(
        sizeof(struct _HashTable)
    ); // Allocates memory for the hash table
    htable->arr =
        size <= 0 ? DEFAULT_SIZE : size; // If size is 0 or less, use default
    htable->list =
        malloc(sizeof(List) * htable->arr); // Allocates array of lists
    for (int i = 0; i < htable->arr; i++)   // For each array slot
    {
        htable->list[i] = list_create(); // Creates a list
    }
    htable->hash = hash == NULL
                       ? default_hash
                       : hash; // Assigns hash function, uses default if NULL
    htable->key_equal =
        key_equal == NULL
            ? default_key_equal
            : key_equal; // Assigns key comparison, uses default if NULL
    htable->size = 0;    // Initializes size
    return htable;       // Returns the hash table
}

void hash_table_destroy(
    HashTable htable,
    void (*key_destroy)(void*),
    void (*value_destroy)(void*)
) // O(n²) - worst case
{
    void destroy_assistent(
        void* element
    ) // Internal helper to destroy hash table elements
    {
        if (key_destroy != NULL) // If key destroy function is not NULL
        {
            key_destroy(((Item)element)->key); // Destroys the key
        }
        if (value_destroy != NULL) // If value destroy function is not NULL
        {
            value_destroy(((Item)element)->value); // Destroys the value
        }
        free(element); // Destroys the item
    }
    for (int i = 0; i < htable->arr; i++) // For each array slot
    {
        list_destroy(
            htable->list[i], destroy_assistent
        ); // Destroys list with destroy callback
    }
    free(htable->list); // Destroys the array of lists
    free(htable);       // Destroys the hash table
}

bool hash_table_is_empty(HashTable htable) // O(1)
{
    return htable->size == 0;
}

int hash_table_size(HashTable htable) // O(1)
{
    return htable->size;
}

void* hash_table_insert(
    HashTable htable,
    void* key,
    void* value
) // O(n) - worst case
{
    int index = htable->hash(key, htable->arr);         // Calculates the index
    list_iterator_start(htable->list[index]);           // Starts the iterator
    while (list_iterator_has_next(htable->list[index])) // While there is a next
    {
        Item listItem = list_iterator_get_next(
            htable->list[index]
        ); // Gets current item and advances
        if (htable->key_equal(listItem->key, key)) // If keys match
        {
            void* oldValue = listItem->value; // Saves old value
            listItem->value = value;          // Updates the value
            return oldValue;                  // Returns old value
        }
    }
    // Key not found
    Item item = malloc(sizeof(struct _Item));    // Allocates item memory
    item->key = key;                             // Assigns the key
    item->value = value;                         // Assigns the value
    list_insert_last(htable->list[index], item); // Inserts item into list
    htable->size++;                              // Increments size
    return NULL;                                 // Returns NULL
}

void* hash_table_remove(
    HashTable htable,
    void* key
) // O(n) - worst case (better to do n + n than copy the entire list_remove_all
  // structure, code is easier to maintain)
{
    int index = htable->hash(key, htable->arr); // Calculates the index
    bool key_equal_assistent(
        void* key, void* element
    ) // Internal key comparison helper
    {
        return htable->key_equal(key, ((Item)element)->key);
    }
    int position = list_find(
        htable->list[index], key_equal_assistent, key
    );                  // Finds element position with comparison function
    if (position != -1) // If position is valid
    {
        Item item =
            list_remove(htable->list[index], position); // Removes the item
        void* value = item->value;                      // Saves the value
        free(item);                                     // Destroys the item
        htable->size--;                                 // Decrements size
        return value;                                   // Returns the value
    }
    return NULL; // Key not found
}

void* hash_table_get(HashTable htable, void* key) // O(n) - worst case
{
    int index = htable->hash(key, htable->arr);         // Calculates the index
    list_iterator_start(htable->list[index]);           // Starts the iterator
    while (list_iterator_has_next(htable->list[index])) // While there is a next
    {
        Item item = list_iterator_get_next(
            htable->list[index]
        );                                     // Gets current item and advances
        if (htable->key_equal(key, item->key)) // If keys match
        {
            return item->value; // Returns the value
        }
    }
    return NULL; // Returns NULL
}

List hash_table_keys(HashTable htable) // O(n²) - worst case
{
    List list = list_create();            // Creates a list
    for (int i = 0; i < htable->arr; i++) // For each array slot
    {
        list_iterator_start(htable->list[i]);           // Starts the iterator
        while (list_iterator_has_next(htable->list[i])) // While there is a next
        {
            Item item = list_iterator_get_next(
                htable->list[i]
            );                                 // Gets current item and advances
            list_insert_last(list, item->key); // Inserts key into list
        }
    }
    return list; // Returns the list
}

List hash_table_values(HashTable htable) // O(n²) - worst case
{
    List list = list_create();            // Creates a list
    for (int i = 0; i < htable->arr; i++) // For each array slot
    {
        list_iterator_start(htable->list[i]);           // Starts the iterator
        while (list_iterator_has_next(htable->list[i])) // While there is a next
        {
            Item item = list_iterator_get_next(
                htable->list[i]
            ); // Gets current item and advances
            list_insert_last(list, item->value); // Inserts value into list
        }
    }
    return list; // Returns the list
}

HashTable hash_table_rehash(
    HashTable htable,
    int new_size,
    void (*key_destroy)(void*),
    void (*value_destroy)(void*)
)
{
    if (new_size > 0) // If new size is valid
    {
        HashTable htableNew = hash_table_create(
            new_size, htable->hash, htable->key_equal
        );                                    // Creates new hash table
        for (int i = 0; i < htable->arr; i++) // For each array slot
        {
            list_iterator_start(htable->list[i]); // Starts the iterator
            while (list_iterator_has_next(
                htable->list[i]
            )) // While there is a next
            {
                Item item = list_iterator_get_next(
                    htable->list[i]
                ); // Gets current item and advances
                hash_table_insert(
                    htableNew, item->key, item->value
                ); // Inserts item into new hash table
            }
        }
        hash_table_destroy(
            htable, key_destroy, value_destroy
        );                // Destroys the old hash table
        return htableNew; // Returns the new hash table
    }
    return htable; // Returns old hash table if size is invalid
}