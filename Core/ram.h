//
// Created by kevin on 19-05-19.
//


#include "../Auxiliar/linked_list.h"
#ifndef LAB3_RAM_H
#define LAB3_RAM_H

struct mem_data{
    char* address;  // 32 Bits
    char* data;  // 32 Bits
};
typedef struct mem_data Mem_data;

struct ram_mem{
    List* data_list;  // Lista enlazada de datos de 32 Bits
    char* addr;  // 32 Bits
    char* read_data;  // 32 Bits
    char* write_data;  // 32 Bits
};
typedef struct ram_mem Ram;

Mem_data* init_mem_data(char *data, char *addr);
Ram* init_ram_mem();
void free_mem_data(void* mem_data);
void free_ram_mem(Ram* ram);

#endif //LAB3_RAM_H
