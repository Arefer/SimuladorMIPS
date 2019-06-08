//
// Created by kevin on 19-05-19.
//


#include "../Auxiliar/linked_list.h"
#ifndef LAB3_RAM_H
#define LAB3_RAM_H


struct ram_mem{
    List* data_list;   // Lista enlazada de datos de 32 Bits
    char* addr;        // 32 Bits
    List* read_data;   // Cola de datos de 32 Bits
    char* write_data;  // 32 Bits
};
typedef struct ram_mem Ram;


Ram* init_ram_mem();
void write_to_ram(Ram* ram);
void read_from_ram(Ram* ram);
void free_ram_mem(Ram* ram);

#endif //LAB3_RAM_H
