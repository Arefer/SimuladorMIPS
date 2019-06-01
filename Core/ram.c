//
// Created by kevin on 19-05-19.
//

#include "ram.h"
#include "../Auxiliar/binary_ops.h"
#include <stdlib.h>
#include <string.h>

/**
 * Inicializa un mem_data
 */
Mem_data* init_mem_data(char *data, char *addr) {
    Mem_data* mem_data = (Mem_data*)malloc(sizeof(Mem_data));
    mem_data->data = data;
    mem_data->address = addr;
    return mem_data;
}

/*
 * Inicializa una memoria ram sin instrucciones
 */
Ram* init_ram_mem(){
    Ram* ram = (Ram*)malloc(sizeof(Ram));
    ram->data_list = init_list();
    ram->addr = decimal_to_binary(0, 32);
    ram->read_data = decimal_to_binary(0, 32);
    ram->write_data = decimal_to_binary(0, 32);
    return ram;
}

/*
 * Libera la memoria de un mem_data
 */
void free_mem_data(void* mem_data){
    Mem_data* data = (Mem_data*) mem_data;
    free(data->data);
    free(data->address);
    free(data);
}

/*
 * Libera la memoria de la memoria ram
 */
void free_ram_mem(Ram* ram){
    free_list_and_nodes(ram->data_list, free_mem_data);
    free(ram->addr);
    free(ram->read_data);
    free(ram->write_data);
    free(ram);
}