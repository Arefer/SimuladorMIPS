//
// Created by kevin on 19-05-19.
//

#include "ram.h"
#include "../Auxiliar/binary_ops.h"
#include "../Auxiliar/custom_string_ops.h"
#include <stdlib.h>
#include <string.h>


/*
 * Inicializa una memoria ram sin instrucciones
 */
Ram* init_ram_mem(){
    Ram* ram = (Ram*)malloc(sizeof(Ram));
    ram->data_list = init_list();
    ram->addr = decimal_to_binary(0, 32);
    ram->read_data = init_list();
    ram->write_data = decimal_to_binary(0, 32);
    return ram;
}


/*
 * Escribe el dato almacenado en write_data en la direccion de memoria addr
 */
void write_to_ram(Ram* ram){
    // Buscamos el nodo en la ram
    char* data = find(ram->data_list, ram->addr);
    // Si no existe, lo agregamos
    if (data == NULL){
        // Copiamos la informacion de write_data
        char* write_data = (char*)malloc(sizeof(strlen(ram->write_data)+1));
        strcpy(write_data, ram->write_data);
        // Copiamos la informacion de addr
        char* addr = (char*)malloc(sizeof(strlen(ram->addr)+1));
        strcpy(addr, ram->addr);
        // Añadimos el dato a la ram
        add_to_list(ram->data_list, write_data, addr);
    } else {  // Si existe, actualizamos su valor
        // Copiamos la informacion de write_data
        char* write_data = (char*)malloc(sizeof(strlen(ram->write_data)+1));
        strcpy(write_data, ram->write_data);
        free(data);
        data = write_data;
    }
}

/*
 * Lee el dato almacenado en la direccion addr y lo copia en read_data
 */
void read_from_ram(Ram* ram){
    // Buscamos el dato en la ram
    char* data = find(ram->data_list, ram->addr);
    if (data != NULL){
        // Copiamos el dato
        char* data_copy = (char*)malloc(sizeof(strlen(data)+1));
        strcpy(data_copy, data);
        // Copiamos el addr
        char* addr = (char*)malloc(sizeof(strlen(ram->addr)+1));
        strcpy(addr, ram->addr);
        // Guardamos el dato en read_data
        push_to_list(ram->read_data, data_copy, addr);
    } else {  // Si no se encuentra, cargar 0
        // Copiamos el addr
        char* addr = (char*)malloc(sizeof(strlen(ram->addr)+1));
        push_to_list(ram->read_data, decimal_to_binary(0, 32), addr);
    }
}

/*
 * Libera la memoria de la memoria ram
 */
void free_ram_mem(Ram* ram){
    free_list_and_nodes(ram->data_list, free_string);
    free(ram->addr);
    free_list_and_nodes(ram->read_data, free_string);
    free(ram->write_data);
    free(ram);
}