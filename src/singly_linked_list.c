#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

typedef struct Node_ *Node;

struct Node_
{
    void *element;
    Node next;
}; // Struct = struct Node_ ; Ponteiro = Node

struct List_
{
    Node head;
    Node tail;
    int size;
    Node current;
}; // Struct = struct List_ ; Ponteiro = List

Node node_create(Node next, void *element) // O(1)
{
    Node node = malloc(sizeof(struct Node_)); // Aloca memória para o node
    node->element = element;                  // Atribui o endereço do elemento
    node->next = next;                        // Atribui o endereço do next
    return node;
    // Me da imenso jeito pois vou usar a função pelo menos umas 3 vezes
}

List list_create() // O(1)
{
    List list = malloc(sizeof(struct List_)); // Aloca memória para a lista
    list->head = NULL;                        // Atribui nenhum endereço para head
    list->tail = NULL;                        // Atribui nenhum endereço para tail
    list->size = 0;                           // Atribui o tamanho da lista para 0
    return list;
}

void list_wipe(List list, void (*free_element)(void *)) // O(n)
{
    Node node = list->head; // Recebe o endereço do node da head
    while (node != NULL)    // Vai da head até a tail
    {
        if (free_element != NULL) // Nem todo elemento em um código necessita de limpeza (descobri isso 4 da manha depois de apanhar pros testes)
        {
            free_element(node->element); // Limpa o elemento
        }
        Node previousNode = node; // Guarda o elemento da node antiga
        node = node->next;        // E avança para uma nova
        free(previousNode);       // Limpa a node antiga
    }
    // É outra função que me da jeito, pois uso duas vezes
}

void list_destroy(List list, void (*free_element)(void *)) // O(n)
{
    list_wipe(list, free_element); // Limpo os nodes e elementos da lista
    free(list);                    // E por fim, limpa a lista
}

bool list_is_empty(List list) // O(1)
{
    return list->size == 0;
}

int list_size(List list) // O(1)
{
    return list->size;
}

void *list_get_first(List list) // O(1)
{
    if (list_is_empty(list)) // Se não tiver nada na lista
    {
        return NULL; // (Ou seja, não tem nenhum elemento definido)
    }
    return list->head->element;
}

void *list_get_last(List list) // O(1)
{
    if (list_is_empty(list)) // Se não tiver nada na lista
    {
        return NULL; // (Ou seja, não tem nenhum elemento definido)
    }
    return list->tail->element;
}

void *list_get(List list, int position) // O(n)
{
    if (position > list_size(list) - 1 || position < 0) // Não pode acessar posições que não existem
    {
        return NULL;
    }
    Node node = list->head;            // A node criada recebe o endereço da head
    for (int i = 0; i < position; i++) // E anda até a posição desejada
    {
        node = node->next; // Anda para frente
    }
    return node->element;
}

int list_find(List list, bool (*equal)(void *, void *), void *element) // O(n)
{
    Node node = list->head;            // O node recebe o endereço da head
    for (int i = 0; node != NULL; i++) // Vai do primeiro ao último node da lista
    {
        if (equal(element, node->element)) // Se os elementos forem iguais
        {
            return i; // Devolve a posição
        }
        node = node->next; // Node recebe o endereço do próximo
    }
    return -1;
}

void list_insert_first(List list, void *element) // O(1)
{
    Node node = node_create(list->head, element); // Cria um node
    list->head = node;                            // E manda para a head
    if (list_is_empty(list))                      // Caso a lista esteja vazia
    {
        list->tail = node; // A tail tambem recebe o node
    }
    list->size++; // Sobe o tamanho da lista
    // Se não tiver nada na lista, é obvio que o endereço do next da head é NULL, logo, tanto a tail quanto a head nesse primeiro elemento, tem o next é definido como NULL
}

void list_insert_last(List list, void *element) // O(1)
{
    Node node = node_create(NULL, element); // Cria um node
    if (list_is_empty(list))                // Caso a lista esteja vazia
    {
        list->head = node; // A head tambem recebe o node
    }
    else // Caso não
    {
        list->tail->next = node; // A tail passa a apontar para node no next
    }
    list->tail = node; // E o node vira o novo tail
    list->size++;      // Sobe o número da lista
}

void list_insert(List list, void *element, int position) // O(n)
{
    if (position < 0 || position > list_size(list)) // Não pode inserir em posições que não existem
    {
        return;
    }
    if (position == 0) // Caso a posição seja da head, ela insere no começo e quebra a função
    {
        list_insert_first(list, element);
        return;
    }
    if (position == list_size(list)) // Caso a posição seja na tail, ela insere no final e quebra a função
    {
        list_insert_last(list, element);
        return;
    }
    Node previousNode = list->head->next;  // Recebe o endereço da head
    for (int i = 1; i < position - 1; i++) // Vai uma node depois da head até o valor anterior da posição
    {
        previousNode = previousNode->next; // Anda para frente
    }
    Node node = node_create(previousNode->next, element); // A nova node aponta para próxima node (a que estava na posição em que foi colocada)
    previousNode->next = node;                            // E a node anterior aponta para a nova node
    list->size++;                                         // Aumenta o tamanho da lista
}

void *list_remove_first(List list) // O(1)
{
    if (list_is_empty(list)) // Se a lista estiver vazia
    {
        return NULL; // Não retorna elemento nenhum
    }
    Node node = list->head;        // Guarda o endereço da node da head
    void *element = node->element; // Guarda o endereço do elemento da head
    list->head = node->next;       // Define o próximo elemento como a head
    free(node);                    // Limpa o primeiro elemento
    list->size--;                  // Diminui o tamanho da lista
    if (list_is_empty(list))       // Se depois disso ela estiver vazia
    {
        list->tail = NULL; // Define a tail como NULL também
    }
    return element; // Por fim, retorna o endereço do elemento
    // Novamente, é implicito que se a lista tiver apenas um elemento e fizermos isso, como a head nova recebe o next da head anterior, esse será NULL
}

void *list_remove_last(List list) // O(n)
{
    if (list_is_empty(list)) // Se a lista estiver vazia
    {
        return NULL; // Não retorna elemento nenhum
    }
    if (list->head->next == NULL) // Se o next da head for NULL, ou seja, tem apenas um elemento
    {
        return list_remove_first(list); // Remove o primeiro (É mais fácil e aparentemente me poupa memória e complexidade)
    }
    Node node = list->head;              // Esse node recebe de primeira instancia o endereço da head
    void *element = list->tail->element; // Guarda o endereço do element da tail
    while (node->next->next != NULL)     // Ele avança na lista até que o next do next seja NULL, ou seja, vai até o penúltimo termo da lista
    {
        node = node->next; // Recebe o endereço do próximo
    }
    free(list->tail);  // Limpa o node da tail
    list->tail = node; // Define o penúltimo node como a tail
    node->next = NULL; // E remove o next do node
    list->size--;      // Diminui o tamanho da lista
    return element;    // Retorna o elemento
}

void *list_remove(List list, int position) // O(n)
{
    if (position < 0 || position > list_size(list) - 1 || list_is_empty(list)) // Não retorna e remove nenhum endereço que não exista na lista
    {
        return NULL;
    }
    if (position == 0 || list->head->next == NULL) // Se a posição for 0 ou ele tiver apenas um elemento
    {
        return list_remove_first(list); // Remove o primeiro
    }
    Node previousNode = list->head->next;  // Recebe o endereço seguinte da head
    for (int i = 1; i < position - 1; i++) // Vai uma node depois da head até o valor anterior da posição
    {
        previousNode = previousNode->next; // Anda para frente
    }
    Node node = previousNode->next;  // Guarda o endereço da node que queremos limpar
    previousNode->next = node->next; // Guarda o endereço da node a seguir da que queremos limpar, unindo assim a lista
    void *element = node->element;   // Guarda o endereço do elemento da node
    free(node);                      // Limpa a node
    list->size--;                    // Diminui o tamanho da lista
    return element;                  // Retorna o elemento
}

void list_make_empty(List list, void (*free_element)(void *)) // O(n)
{
    list_wipe(list, free_element); // Limpo os nodes e elementos da lista
    list->head = NULL;             // Depois defino tudo como no começo
    list->tail = NULL;
    list->size = 0;
}

void list_to_array(List list, void **out_array)
{
    Node node = list->head;            // Recebe o endereço da head
    for (int i = 0; node != NULL; i++) // Percorre do primeiro node até o ponto em que ele se torna nulo
    {
        out_array[i] = node->element; // Adiciona o elemento na array
        node = node->next;            // Avança para o próximo
    }
}

int list_count_all(List list, bool (*equal)(void *, void *), void *element) // O(n)
{
    Node node = list->head; // Recebe o endereço da head
    int i = 0;              // Define o counter a 0
    while (node != NULL)    // Vai do primeiro ao último termo
    {
        if (equal(element, node->element)) // Se for igual
        {
            i++; // Acrescenta no counter
        }
        node = node->next; // Avança na lista
    }
    return i; // Retorna o counter
}

int list_remove_all(List list, bool (*equal_element)(void *, void *), void (*free_element)(void *), void *element) // O(n) - A mais dificil até então
{
    int occurrences = 0;      // Inicia o contador de ocorrencias do elemento
    Node node = list->head;   // Recebe o endereço da head
    Node previousNode = NULL; // Vai servir mais pra frente para guardar o node anterior
    while (node != NULL)      // Percorre toda a lista
    {
        if (equal_element(node->element, element)) // Se o node atual for igual ao elemento
        {
            occurrences++;            // Ele acrescenta no contador
            if (previousNode != NULL) // Se a previousNode for diferente de NULL (ou seja, não estamos na head)
            {
                previousNode->next = node->next; // o next do previousNode vai receber o próximo do node (porque vamos apagar o node, e precisamos ligar o próximo dele no anterior)
            }
            else // Caso seja a head
            {
                list->head = node->next; // Ele define o próximo node como a head (ja que vamos apagar a head)
            }
            if (node->next == NULL) // E caso o node next seja null (estamos na tail)
            {
                list->tail = previousNode; // O anterior ao node vai ser a tail
            }
            if (free_element != NULL) // Caso free_element exista
            {
                free_element(node->element); // Limpamos o elemento
            }
            Node nextNode = node->next; // Guardamos o próximo node em uma variável temporária
            free(node);                 // Damos free da node pretendida
            node = nextNode;            // Movemos a node para o próximo
            list->size--;               // Diminuimos o tamanho da lista
        }
        else // Caso a verificação seja errada
        {
            previousNode = node; // previousNode vai ser igual ao node
            node = node->next;   // E o node anda para frente
        }
    }
    return occurrences;
}

int list_remove_duplicates(List list, bool (*equal_element)(void *, void *), void (*free_element)(void *), void *element) // O(n)
{
    int occurrences = 0;      // Inicia o contador de ocorrencias do elemento
    Node node = list->head;   // Recebe o endereço da head
    Node previousNode = NULL; // Vai servir mais pra frente para guardar o node anterior
    while (node != NULL)      // Percorre toda a lista
    {
        if (equal_element(node->element, element)) // Se o node atual for igual ao elemento
        {
            occurrences++; // Ele acrescenta no contador
            if (occurrences > 1)
            {
                previousNode->next = node->next; // o next do previousNode vai receber o próximo do node (tirei o if pois nunca vou apagar a head)
                if (node->next == NULL)          // E caso o node next seja null (estamos na tail)
                {
                    list->tail = previousNode; // O anterior ao node vai ser a tail
                }
                if (free_element != NULL) // Caso free_element exista
                {
                    free_element(node->element); // Limpamos o elemento
                }
                Node nextNode = node->next; // Guardamos o próximo node em uma variável temporária
                free(node);                 // Damos free da node pretendida
                node = nextNode;            // Movemos a node para o próximo
                list->size--;               // Diminuimos o tamanho da lista
            }
        }
        else // Caso a verificação seja errada
        {
            previousNode = node; // previousNode vai ser igual ao node
            node = node->next;   // E o node anda para frente
        }
    }
    return occurrences - 1; // Retorna apenas as ocorrencias após a primeira
}

List list_join(List list1, List list2) // O(n)
{
    List list = list_create(); // Cria a nova lista
    Node node = list1->head;   // node recebe o endereço da head da lista 1
    while (node != NULL)       // Percorre toda a lista 1 adicionando os elementos para a nova lista
    {
        list_insert_last(list, node->element); // Insere o elemento na lista
        node = node->next;                     // Anda para o próximo
    }
    node = list2->head;  // node recebe o endereço da head da lista 2
    while (node != NULL) // Percorre toda a lista 2 adicionando os elementos para a nova lista
    {
        list_insert_last(list, node->element); // Insere o elemento na lista
        node = node->next;                     // Anda para o próximo
    }
    return list;
}

void list_print(List list, void (*print_element)(void *element)) // O(n)
{
    Node node = list->head; // Recebe o endereço da head
    while (node != NULL)    // Percorre toda a lista
    {
        print_element(node->element); // Printa o elemento do node atual
        node = node->next;            // Anda para o próximo
    }
}

List list_get_sublist_between(List list, int start_idx, int end_idx) // O(n)
{
    if (start_idx < 0 || start_idx > list_size(list) - 1 || end_idx < 0 || end_idx > list_size(list) - 1) // Caso os indices sejam invalidos
    {
        return NULL;
    }
    List newlist = list_create();       // Crio uma nova lista
    Node node = list->head;             // Recebe o endereço da minha lista recebida
    for (int i = 0; i < start_idx; i++) // Percorro até o start_idx da lista recebida
    {
        node = node->next; // Anda para o próximo
    }
    for (int i = start_idx; i <= end_idx; i++) // Quando chego no meu start_idx, vou até meu end_idx (já foi corrigida a diferença de índice no loop de cima, então posso usar o <=)
    {
        list_insert_last(newlist, node->element); // Insere o elemento do atual na minha nova lista
        node = node->next;                        // Anda para o próximo
    }
    return newlist; // Retorno a nova lista
}

List list_get_sublist(List list, int indexes[], int count) // O(n)
{
    List newlist = list_create();                        // Cria minha nova lista
    bool *index = calloc(list_size(list), sizeof(bool)); // Criamos um array de booleanos do tamanho da lista (Usamos calloc para definir o espaço alocado como zero, que em booleano vai significar false)
    for (int i = 0; i < count; i++)                      // Percorre os elementos da array indexes
    {
        if (indexes[i] >= 0 && indexes[i] <= list_size(list) - 1)
        {
            index[indexes[i]] = true; // E para cada posição que mostra o array indexes, colocamos true no nosso array de booleanos
        }
    }
    Node node = list->head;                         // Recebe o endereço da head
    int j = 0;                                      // Vamos usar essa váriavel para contabilizar o número necessário de passadas na lista (se o tamanho do array indexes terminar, não vale a pena continuar rodando)
    for (int i = 0; node != NULL && j < count; i++) // Percorre a lista até onde seja necessário (só é necessario até o momento em que j>=count)
    {
        if (index[i]) // Se na posição i o elemento do array for true
        {
            list_insert_last(newlist, node->element); // O nosso novo array recebe o elemento da posição
            j++;                                      // Acrescenta um valor para j
        }
        node = node->next; // Anda para frente
    }
    free(index);    // Limpamos o array de booleanos
    return newlist; // Retorna a nova lista
}

List list_map(List list, void *(*func)(void *)) // O(n)
{
    List newlist = list_create(); // Cria uma nova lista
    Node node = list->head;       // Recebe o endereço da head
    while (node != NULL)          // Percorre toda a lista
    {
        list_insert_last(newlist, func(node->element)); // Insere o elemento modificado pela função na lista
        node = node->next;                              // Anda para o próximo
    }
    return newlist;
}

List list_filter(List list, bool (*func)(void *)) // O(n)
{
    List newlist = list_create(); // Cria a lista
    Node node = list->head;       // Recebe o endereço da head
    while (node != NULL)          // Percorre toda a lista
    {
        if (func(node->element)) // Se a função retornar true para o elemento do node
        {
            list_insert_last(newlist, node->element); // Insere esse elemento na lista
        }
        node = node->next; // Anda para o próximo
    }
    return newlist;
}

// Iteradores

void list_iterator_start(List list) // O(1)
{
    list->current = list->head; // O current recebe o endereço da head
}

bool list_iterator_has_next(List list) // O(1)
{
    return list->current != NULL; // Se o current for diferente de NULL, ele tem um próximo
}

void *list_iterator_get_next(List list) // O(1)
{
    void *element = list->current->element; // Guarda o endereço do elemento
    list->current = list->current-> next; // Anda para o próximo
    return element; // Retorna o elemento
}