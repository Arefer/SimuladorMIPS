//
// Created by kevin on 19-05-19.
//

#ifndef LAB3_LINKED_LIST_H
#define LAB3_LINKED_LIST_H

struct node{
    char* id;
    void* data;
    struct node* next;
};
typedef struct node Node;


struct list{
    Node* first;
    Node* last;
    int length;
};
typedef struct list List;

List* init_list();
Node* init_node(void* data, char* id);
void add_to_list(List* list, void* data, char* id);
void push_to_list(List* list, void* data, char* id);
void* pop_from_list(List* list);
void iterate(List* list, void (*f)(void*));
void* find(List* list, char* id);
void free_list(List* list);
void free_list_and_nodes(List* list, void (*custom_data_free)(void*), int erase_id);
#endif //LAB3_LINKED_LIST_H
