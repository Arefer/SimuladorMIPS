//
// Created by kevin on 19-05-19.
//

#include "linked_list.h"
#include <stdlib.h>

/*
 * Inicializa una lista enlazada
 */
List* init_list(){
    List* list = (List*)malloc(sizeof(List));
    list->first = NULL;
    list->last = NULL;
    list->length = 0;
    return list;
}

/*
 * Inicializa un nodo
 */
Node* init_node(void* data, char* id){
    Node* n = (Node*)malloc(sizeof(Node));
    n->id = id;
    n->data = data;
    n->next = NULL;
    return n;
}

/*
 * Añade un elemento a la lista
 */
void add_to_list(List* list, void* data, char* id){
    // Creamos el nodo con la informacion
    Node* n = init_node(data, id);
    if (list->length == 0){
        list->first = n;
        list->last = n;
    } else {
        list->last->next = n;
        list->last = n;
    }
    list->length += 1;
}

/*
 * Itera sobre la lista y aplica la funcion especificada
 */
void iterate(List* list, void (*f)(void*)){
    Node* cursor = list->first;
    while (cursor != NULL){
        (*f)(cursor->data);
        cursor = cursor->next;
    }
}

/*
 * Libera la lista enlazada sin liberar la informacion de los nodos
 */
void free_list(List* list){
    Node* cursor = list->first;
    while (cursor != NULL){
        Node* next = cursor->next;
        free(cursor);
        cursor = next;
    }
    free(list);
}

/*
 * Libera la lista enlazada inclyendo la informacion de los nodos
 */
void free_list_and_nodes(List* list, void (*custom_data_free)(void*)){
    Node* cursor = list->first;
    while (cursor != NULL){
        Node* next = cursor->next;
        (*custom_data_free)(cursor->data);
        free(cursor);
        cursor = next;
    }
    free(list);
}