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
Mem_data *init_mem_data(char *data, char *addr) {
    Mem_data* mem_data = (Mem_data*)malloc(sizeof(Mem_data));
    mem_data->data = data;
    mem_data->address = addr;
    return mem_data;
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