//
// Created by kevin on 19-05-19.
//

#include "linked_list.h"
#include <stdlib.h>
#include <string.h>
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
 * Añade un elemento al final de la lista
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
 * Añade un elemento al principio de la lista
 */
void push_to_list(List* list, void* data, char* id){
    // Creamos el nodo con la información
    Node* n = init_node(data, id);
    if (list->length == 0){
        list->first = n;
        list->last = n;
    } else {
        Node* aux = list->first;
        list->first = n;
        n->next = aux;
    }
    list->length += 1;
}

/*
 * Extrae el ultimo elemento de la lista (libera el nodo pero no su informacion)
 */
void* pop_from_list(List* list){
    Node* last = list->last;
    Node* cursor = list->first;
    while (cursor->next != list->last){
        cursor = cursor->next;
    }
    list->last = cursor;
    void* data = last->data;
    free(last->id);
    free(last);
    return data;
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
 * Busca un elemento en la lista
 */
void* find(List* list, char* id){
    Node* cursor = list->first;
    while (cursor != NULL){
        if (strcmp(cursor->id, id) == 0){
            return cursor->data;
        }
        cursor = cursor->next;
    }
    return NULL;
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

/**
 * Libera la lista enlazada incluyendo la información de los nodos
 * @param list La lista enlazada
 * @param custom_data_free: funcion que libera Node->data
 * @param erase_id: indica si se debe hacer un free al id (1 = free, otro numero = no free)
 */
void free_list_and_nodes(List* list, void (*custom_data_free)(void*), int erase_id){
    Node* cursor = list->first;
    while (cursor != NULL){
        Node* next = cursor->next;
        (*custom_data_free)(cursor->data);
        if (erase_id == 1) free(cursor->id);
        free(cursor);
        cursor = next;
    }
    free(list);
}