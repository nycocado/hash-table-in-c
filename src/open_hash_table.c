#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "list.h"

// Definições de tipos de dados
typedef struct _Item *Item;
typedef int (*Hash)(void *, int);
typedef bool (*Key_Equal)(void *, void *);

// Estruturas de dados
struct _HashTable
{
    int size;
    int arr;
    Hash hash;
    Key_Equal key_equal;
    List *list;
};

struct _Item
{
    void *key;
    void *value;
};

// Funções de Apoio
int default_hash(void *key, int n) // Função padrão para calcular a hash - O(n) - n sendo o número de caracteres
{
    int result = 0;                                // Initializa o resultado
    int a = 127;                                   // 127 é um número primo, recomendado para esses tipos de operações
    for (int i = 0; ((char *)key)[i] != '\0'; i++) // Enquanto tiver caracteres
    {
        result = (result * a + ((char *)key)[i]); // Vai fazer a operação matemática
    }
    return result % n; // Resultado do indíce
}

bool default_key_equal(void *key1, void *key2) // Função padrão para comparar chaves - O(1)
{
    return strcmp((char *)key1, (char *)key2) == 0;
}

// Funções de HashTable
HashTable hash_table_create(int size, int (*hash)(void *, int), bool (*key_equal)(void *, void *)) // O(n) - no pior caso
{
    HashTable htable = malloc(sizeof(struct _HashTable)); // Aloca memória para a hash table
    htable->arr = size <= 0 ? DEFAULT_SIZE : size;        // Se o tamanho for menor ou igual a zero, usa o tamanho padrão
    htable->list = malloc(sizeof(List) * htable->arr);    // Aloca memória para o array de listas
    for (int i = 0; i < htable->arr; i++)                 // Para cada espaço do array
    {
        htable->list[i] = list_create(); // Cria uma lista
    }
    htable->hash = hash == NULL ? default_hash : hash;                     // Atribui a função de hash, se for nula, usa a padrão
    htable->key_equal = key_equal == NULL ? default_key_equal : key_equal; // Atribui a função de comparação de chaves, se for nula, usa a padrão
    htable->size = 0;                                                      // Inicializa o tamanho
    return htable;                                                         // Retorna a hash table
}

void hash_table_destroy(HashTable htable, void (*key_destroy)(void *), void (*value_destroy)(void *)) // O(n²) - no pior caso
{
    void destroy_assistent(void *element) // Função interna para destruir os elementos da hash table
    {
        if (key_destroy != NULL) // Se a função de destruição de chaves não for nula
        {
            key_destroy(((Item)element)->key); // Destrói a chave
        }
        if (value_destroy != NULL) // Se a função de destruição de valores não for nula
        {
            value_destroy(((Item)element)->value); // Destrói o valor
        }
        free(element); // Destrói o item
    }
    for (int i = 0; i < htable->arr; i++) // Para cada espaço do array
    {
        list_destroy(htable->list[i], destroy_assistent); // Destrói a lista, passando a função de destruição
    }
    free(htable->list); // Destrói o array de listas
    free(htable);       // Destrói a hash table
}

bool hash_table_is_empty(HashTable htable) // O(1)
{
    return htable->size == 0;
}

int hash_table_size(HashTable htable) // O(1)
{
    return htable->size;
}

void *hash_table_insert(HashTable htable, void *key, void *value) // O(n) - no pior caso
{
    int index = htable->hash(key, htable->arr);         // Calcula o indíce
    list_iterator_start(htable->list[index]);           // Inicia o iterador
    while (list_iterator_has_next(htable->list[index])) // Enquanto tiver próximo
    {
        Item listItem = list_iterator_get_next(htable->list[index]); // Pega o item atual e passa para a próxima
        if (htable->key_equal(listItem->key, key))                   // Se a chave for igual
        {
            void *oldValue = listItem->value; // Salva o valor antigo
            listItem->value = value;          // Atualiza o valor
            return oldValue;                  // Retorna o valor antigo
        }
    }
    // Se não tiver a chave
    Item item = malloc(sizeof(struct _Item));    // Aloca memória para o item
    item->key = key;                             // Atribui a chave
    item->value = value;                         // Atribui o valor
    list_insert_last(htable->list[index], item); // Insere o item na lista
    htable->size++;                              // Incrementa o tamanho
    return NULL;                                 // Retorna nulo
}

void *hash_table_remove(HashTable htable, void *key) // O(n) - no pior caso (é melhor fazer n + n do que copiar aquela estrutura inteira do list remove all na minha opinião, a manutenção do código é mais simples)
{
    int index = htable->hash(key, htable->arr);        // Calcula o indíce
    bool key_equal_assistent(void *key, void *element) // Função interna para comparar chaves
    {
        return htable->key_equal(key, ((Item)element)->key);
    }
    int position = list_find(htable->list[index], key_equal_assistent, key); // Procura a posição do elemento, passando a função de comparação e a chave
    if (position != -1)                                                      // Se a posição for diferente de -1
    {
        Item item = list_remove(htable->list[index], position); // Remove o item
        void *value = item->value;                              // Salva o valor
        free(item);                                             // Destrói o item
        htable->size--;                                         // Decrementa o tamanho
        return value;                                           // Retorna o valor
    }
    return NULL; // Caso não encontre, retorna nulo
}

void *hash_table_get(HashTable htable, void *key) // O(n) - no pior caso
{
    int index = htable->hash(key, htable->arr);         // Calcula o indíce
    list_iterator_start(htable->list[index]);           // Inicia o iterador
    while (list_iterator_has_next(htable->list[index])) // Enquanto tiver próximo
    {
        Item item = list_iterator_get_next(htable->list[index]); // Pega o item atual e passa para o próximo
        if (htable->key_equal(key, item->key))                   // Se a chave for igual
        {
            return item->value; // Retorna o valor
        }
    }
    return NULL; // Retorna nulo
}

List hash_table_keys(HashTable htable) // O(n²) - no pior caso
{
    List list = list_create();            // Cria uma lista
    for (int i = 0; i < htable->arr; i++) // Para cada espaço do array
    {
        list_iterator_start(htable->list[i]);           // Inicia o iterador
        while (list_iterator_has_next(htable->list[i])) // Enquanto tiver próximo
        {
            Item item = list_iterator_get_next(htable->list[i]); // Pega o item atual e passa para o próximo
            list_insert_last(list, item->key);                   // Insere a chave na lista
        }
    }
    return list; // Retorna a lista
}

List hash_table_values(HashTable htable) // O(n²) - no pior caso
{
    List list = list_create();            // Cria uma lista
    for (int i = 0; i < htable->arr; i++) // Para cada espaço do array
    {
        list_iterator_start(htable->list[i]);           // Inicia o iterador
        while (list_iterator_has_next(htable->list[i])) // Enquanto tiver próximo
        {
            Item item = list_iterator_get_next(htable->list[i]); // Pega o item atual e passa para o próximo
            list_insert_last(list, item->value);                 // Insere o valor na lista
        }
    }
    return list; // Retorna a lista
}

HashTable hash_table_rehash(HashTable htable, int new_size, void (*key_destroy)(void *), void (*value_destroy)(void *))
{
    if (new_size > 0) // Se o novo tamanho for maior que zero
    {
        HashTable htableNew = hash_table_create(new_size, htable->hash, htable->key_equal); // Cria uma nova hash table
        for (int i = 0; i < htable->arr; i++)                                               // Para cada espaço do array
        {
            list_iterator_start(htable->list[i]);           // Inicia o iterador
            while (list_iterator_has_next(htable->list[i])) // Enquanto tiver próximo
            {
                Item item = list_iterator_get_next(htable->list[i]);  // Pega o item atual e passa para o próximo
                hash_table_insert(htableNew, item->key, item->value); // Insere o item na nova hash table
            }
        }
        hash_table_destroy(htable, key_destroy, value_destroy); // Destrói a hash table antiga
        return htableNew;                                       // Retorna a nova hash table
    }
    return htable; // Caso o novo tamanho seja menor ou igual a zero, retorna a hash table antiga
}