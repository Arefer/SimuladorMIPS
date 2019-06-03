//
// Created by kevin on 19-05-19.
//

#ifndef LAB3_INSTR_MEM_H
#define LAB3_INSTR_MEM_H

#include "../Auxiliar/linked_list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern const char* NOP;
extern const char* ADD;
extern const char* SUB;
extern const char* ADDI;
extern const char* SUBI;
extern const char* BEQ;
extern const char* BNE;
extern const char* J;
extern const char* LW;
extern const char* SW;

struct instruction{
    int line;
    char* address;  // 32 Bits
    const char* name;
    char type;
    char* rs;  // 5 Bits
    char* rt;  // 5 Bits
    char* rd;  // Tipo R, 5 Bits
    int immediate;  // Tipo I
    char* j_address;  // Address de instruccion "j" y immediate de instrucciones "beq" y "bne"
    char* label;
};
typedef struct instruction Instruction;


Instruction* init_instr();
Instruction* init_nop_instr();
Instruction* search_by_label(List* instr_mem, char* label);
void print_instr(void* instr);
void free_instr(void* instruction);
#endif //LAB3_INSTR_MEM_H
