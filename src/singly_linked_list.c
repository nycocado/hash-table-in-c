#include "list.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct Node_* Node;

struct Node_
{
        void* element;
        Node next;
}; // Struct = struct Node_ ; Pointer = Node

struct List_
{
        Node head;
        Node tail;
        int size;
        Node current;
}; // Struct = struct List_ ; Pointer = List

Node node_create(Node next, void* element) // O(1)
{
    Node node = malloc(sizeof(struct Node_)); // Allocates memory for the node
    node->element = element;                  // Assigns element address
    node->next = next;                        // Assigns next address
    return node;
    // Useful since this function is called at least 3 times
}

List list_create() // O(1)
{
    List list = malloc(sizeof(struct List_)); // Allocates memory for the list
    list->head = NULL;                        // Sets head to NULL
    list->tail = NULL;                        // Sets tail to NULL
    list->size = 0;                           // Sets size to 0
    return list;
}

void list_wipe(List list, void (*free_element)(void*)) // O(n)
{
    Node node = list->head; // Gets node address from head
    while (node != NULL)    // Traverses from head to tail
    {
        if (free_element != NULL) // Not every element needs cleanup (discovered
                                  // this at 4am after struggling with tests)
        {
            free_element(node->element); // Cleans the element
        }
        Node previousNode = node; // Saves the old node
        node = node->next;        // Advances to the next
        free(previousNode);       // Cleans the old node
    }
    // Another useful function, used twice
}

void list_destroy(List list, void (*free_element)(void*)) // O(n)
{
    list_wipe(list, free_element); // Cleans the nodes and elements of the list
    free(list);                    // Finally, cleans the list
}

bool list_is_empty(List list) // O(1)
{
    return list->size == 0;
}

int list_size(List list) // O(1)
{
    return list->size;
}

void* list_get_first(List list) // O(1)
{
    if (list_is_empty(list)) // If the list is empty
    {
        return NULL; // (i.e., no element is defined)
    }
    return list->head->element;
}

void* list_get_last(List list) // O(1)
{
    if (list_is_empty(list)) // If the list is empty
    {
        return NULL; // (i.e., no element is defined)
    }
    return list->tail->element;
}

void* list_get(List list, int position) // O(n)
{
    if (position > list_size(list) - 1 ||
        position < 0) // Cannot access positions that do not exist
    {
        return NULL;
    }
    Node node = list->head;            // Created node receives the head address
    for (int i = 0; i < position; i++) // Walks to the desired position
    {
        node = node->next; // Moves forward
    }
    return node->element;
}

int list_find(List list, bool (*equal)(void*, void*), void* element) // O(n)
{
    Node node = list->head;            // Node receives head address
    for (int i = 0; node != NULL; i++) // Traverses from first to last node
    {
        if (equal(element, node->element)) // If elements are equal
        {
            return i; // Returns position
        }
        node = node->next; // Node receives the next address
    }
    return -1;
}

void list_insert_first(List list, void* element) // O(1)
{
    Node node = node_create(list->head, element); // Creates a node
    list->head = node;                            // Sets as head
    if (list_is_empty(list))                      // If the list is empty
    {
        list->tail = node; // Tail also receives the node
    }
    list->size++; // Increments list size
    // If the list is empty, the next of the head is obviously NULL, so both
    // tail and head for this first element have next defined as NULL
}

void list_insert_last(List list, void* element) // O(1)
{
    Node node = node_create(NULL, element); // Creates a node
    if (list_is_empty(list))                // If the list is empty
    {
        list->head = node; // Head also receives the node
    }
    else // Otherwise
    {
        list->tail->next = node; // Tail's next points to node
    }
    list->tail = node; // Node becomes the new tail
    list->size++;      // Increments list size
}

void list_insert(List list, void* element, int position) // O(n)
{
    if (position < 0 ||
        position >
            list_size(list)) // Cannot insert at positions that do not exist
    {
        return;
    }
    if (position ==
        0) // If position is head, inserts at the beginning and returns
    {
        list_insert_first(list, element);
        return;
    }
    if (position ==
        list_size(list)) // If position is tail, inserts at the end and returns
    {
        list_insert_last(list, element);
        return;
    }
    Node previousNode = list->head->next; // Receives the head address
    for (int i = 1; i < position - 1;
         i++) // Moves one node after head up to the node before the position
    {
        previousNode = previousNode->next; // Moves forward
    }
    Node node = node_create(
        previousNode->next, element
    ); // New node points to the next node (the one previously at the target
       // position)
    previousNode->next = node; // Previous node points to the new node
    list->size++;              // Increases list size
}

void* list_remove_first(List list) // O(1)
{
    if (list_is_empty(list)) // If the list is empty
    {
        return NULL; // Returns no element
    }
    Node node = list->head;        // Saves the head node address
    void* element = node->element; // Saves the head element address
    list->head = node->next;       // Sets next element as head
    free(node);                    // Frees the first element
    list->size--;                  // Decrements list size
    if (list_is_empty(list))       // If the list becomes empty after this
    {
        list->tail = NULL; // Sets tail to NULL as well
    }
    return element; // Returns the element address
    // Again, if the list has only one element, the new head receives the next
    // of the previous head, which will be NULL
}

void* list_remove_last(List list) // O(n)
{
    if (list_is_empty(list)) // If the list is empty
    {
        return NULL; // Returns no element
    }
    if (list->head->next ==
        NULL) // If head's next is NULL, i.e., only one element exists
    {
        return list_remove_first(
            list
        ); // Removes the first (easier and saves memory and complexity)
    }
    Node node = list->head; // This node initially receives the head address
    void* element = list->tail->element; // Saves the tail element address
    while (node->next->next != NULL)     // Advances until next->next is NULL,
                                         // reaching the second-to-last node
    {
        node = node->next; // Receives the next address
    }
    free(list->tail);  // Frees the tail node
    list->tail = node; // Sets second-to-last node as tail
    node->next = NULL; // Removes the node's next pointer
    list->size--;      // Decrements list size
    return element;    // Returns element
}

void* list_remove(List list, int position) // O(n)
{
    if (position < 0 || position > list_size(list) - 1 ||
        list_is_empty(list)) // Does not return or remove any address that does
                             // not exist in the list
    {
        return NULL;
    }
    if (position == 0 ||
        list->head->next ==
            NULL) // If position is 0 or list has only one element
    {
        return list_remove_first(list); // Removes the first
    }
    Node previousNode = list->head->next; // Receives the address after head
    for (int i = 1; i < position - 1;
         i++) // Moves one node after head up to the node before the position
    {
        previousNode = previousNode->next; // Moves forward
    }
    Node node = previousNode->next;  // Saves the address of the node to remove
    previousNode->next = node->next; // Links the previous node to the next,
                                     // reconnecting the list
    void* element = node->element;   // Saves the node's element address
    free(node);                      // Frees the node
    list->size--;                    // Decrements list size
    return element;                  // Returns element
}

void list_make_empty(List list, void (*free_element)(void*)) // O(n)
{
    list_wipe(list, free_element); // Cleans the nodes and elements of the list
    list->head = NULL;             // Resets everything to initial state
    list->tail = NULL;
    list->size = 0;
}

void list_to_array(List list, void** out_array)
{
    Node node = list->head;            // Receives the head address
    for (int i = 0; node != NULL; i++) // Traverses from first node until null
    {
        out_array[i] = node->element; // Adds element to the array
        node = node->next;            // Advances to the next
    }
}

int list_count_all(
    List list,
    bool (*equal)(void*, void*),
    void* element
) // O(n)
{
    Node node = list->head; // Receives the head address
    int i = 0;              // Sets counter to 0
    while (node != NULL)    // Traverses from first to last element
    {
        if (equal(element, node->element)) // If equal
        {
            i++; // Increments counter
        }
        node = node->next; // Advances in the list
    }
    return i; // Returns the counter
}

int list_remove_all(
    List list,
    bool (*equal_element)(void*, void*),
    void (*free_element)(void*),
    void* element
) // O(n) - The hardest one so far
{
    int occurrences = 0;      // Initializes occurrence counter
    Node node = list->head;   // Receives the head address
    Node previousNode = NULL; // Will be used later to track the previous node
    while (node != NULL)      // Traverses the entire list
    {
        if (equal_element(
                node->element, element
            )) // If current node equals the element
        {
            occurrences++; // Increments the counter
            if (previousNode !=
                NULL) // If previousNode is not NULL (i.e., not at the head)
            {
                previousNode->next =
                    node->next; // previousNode's next receives the node's next
                                // (to reconnect the list after removal)
            }
            else // If at the head
            {
                list->head = node->next; // Sets next node as head (since head
                                         // is being removed)
            }
            if (node->next == NULL) // If node's next is null (at the tail)
            {
                list->tail = previousNode; // Previous node becomes the tail
            }
            if (free_element != NULL) // If free_element is not NULL
            {
                free_element(node->element); // Cleans the element
            }
            Node nextNode =
                node->next;  // Saves the next node in a temporary variable
            free(node);      // Frees the target node
            node = nextNode; // Moves to the next node
            list->size--;    // Decrements list size
        }
        else // If the condition is not met
        {
            previousNode = node; // previousNode becomes the current node
            node = node->next;   // Node advances forward
        }
    }
    return occurrences;
}

int list_remove_duplicates(
    List list,
    bool (*equal_element)(void*, void*),
    void (*free_element)(void*),
    void* element
) // O(n)
{
    int occurrences = 0;      // Initializes occurrence counter
    Node node = list->head;   // Receives the head address
    Node previousNode = NULL; // Will be used later to track the previous node
    while (node != NULL)      // Traverses the entire list
    {
        if (equal_element(
                node->element, element
            )) // If current node equals the element
        {
            occurrences++; // Increments the counter
            if (occurrences > 1)
            {
                previousNode->next =
                    node->next; // previousNode's next receives the node's next
                                // (no if needed since head is never removed
                                // here)
                if (node->next == NULL) // If node's next is null (at the tail)
                {
                    list->tail = previousNode; // Previous node becomes the tail
                }
                if (free_element != NULL) // If free_element is not NULL
                {
                    free_element(node->element); // Cleans the element
                }
                Node nextNode =
                    node->next;  // Saves the next node in a temporary variable
                free(node);      // Frees the target node
                node = nextNode; // Moves to the next node
                list->size--;    // Decrements list size
            }
        }
        else // If the condition is not met
        {
            previousNode = node; // previousNode becomes the current node
            node = node->next;   // Node advances forward
        }
    }
    return occurrences - 1; // Returns only the occurrences after the first
}

List list_join(List list1, List list2) // O(n)
{
    List list = list_create(); // Creates the new list
    Node node = list1->head;   // Node receives head address of list 1
    while (node != NULL) // Traverses list 1 adding elements to the new list
    {
        list_insert_last(list, node->element); // Inserts element into list
        node = node->next;                     // Moves to the next
    }
    node = list2->head;  // Node receives head address of list 2
    while (node != NULL) // Traverses list 2 adding elements to the new list
    {
        list_insert_last(list, node->element); // Inserts element into list
        node = node->next;                     // Moves to the next
    }
    return list;
}

void list_print(List list, void (*print_element)(void* element)) // O(n)
{
    Node node = list->head; // Receives the head address
    while (node != NULL)    // Traverses the entire list
    {
        print_element(node->element); // Prints the current node's element
        node = node->next;            // Moves to the next
    }
}

List list_get_sublist_between(List list, int start_idx, int end_idx) // O(n)
{
    if (start_idx < 0 || start_idx > list_size(list) - 1 || end_idx < 0 ||
        end_idx > list_size(list) - 1) // If indices are invalid
    {
        return NULL;
    }
    List newlist = list_create(); // Creates a new list
    Node node = list->head;       // Receives the address of the given list
    for (int i = 0; i < start_idx;
         i++) // Traverses to start_idx of the given list
    {
        node = node->next; // Moves to the next
    }
    for (int i = start_idx; i <= end_idx;
         i++) // Once at start_idx, iterate to end_idx (index offset already
              // corrected in the loop above, so <= can be used)
    {
        list_insert_last(
            newlist, node->element
        );                 // Inserts current element into the new list
        node = node->next; // Moves to the next
    }
    return newlist; // Returns the new list
}

List list_get_sublist(List list, int indexes[], int count) // O(n)
{
    List newlist = list_create(); // Creates the new list
    bool* index = calloc(
        list_size(list), sizeof(bool)
    ); // Creates a boolean array of list size (calloc initializes allocated
       // space to zero, which means false for booleans)
    for (int i = 0; i < count; i++) // Traverses elements of the indexes array
    {
        if (indexes[i] >= 0 && indexes[i] <= list_size(list) - 1)
        {
            index[indexes[i]] = true; // Sets true in the boolean array for each
                                      // position indicated by indexes
        }
    }
    Node node = list->head; // Receives the head address
    int j = 0; // Tracks the number of necessary passes through the list (stops
               // once all indexed elements are found)
    for (int i = 0; node != NULL && j < count;
         i++) // Traverses the list only as far as necessary (stops when j >=
              // count)
    {
        if (index[i]) // If the element at position i in the boolean array is
                      // true
        {
            list_insert_last(
                newlist, node->element
            );   // Adds the element at the position to the new list
            j++; // Increments j
        }
        node = node->next; // Moves forward
    }
    free(index);    // Frees the boolean array
    return newlist; // Returns the new list
}

List list_map(List list, void* (*func)(void*)) // O(n)
{
    List newlist = list_create(); // Creates a new list
    Node node = list->head;       // Receives the head address
    while (node != NULL)          // Traverses the entire list
    {
        list_insert_last(
            newlist, func(node->element)
        ); // Inserts the element modified by the function into the list
        node = node->next; // Moves to the next
    }
    return newlist;
}

List list_filter(List list, bool (*func)(void*)) // O(n)
{
    List newlist = list_create(); // Creates the list
    Node node = list->head;       // Receives the head address
    while (node != NULL)          // Traverses the entire list
    {
        if (func(
                node->element
            )) // If the function returns true for the node's element
        {
            list_insert_last(
                newlist, node->element
            ); // Inserts this element into the list
        }
        node = node->next; // Moves to the next
    }
    return newlist;
}

// Iterators

void list_iterator_start(List list) // O(1)
{
    list->current = list->head; // Current receives the head address
}

bool list_iterator_has_next(List list) // O(1)
{
    return list->current !=
           NULL; // If current is not NULL, there is a next element
}

void* list_iterator_get_next(List list) // O(1)
{
    void* element = list->current->element; // Saves the element address
    list->current = list->current->next;    // Moves to the next
    return element;                         // Returns the element
}
