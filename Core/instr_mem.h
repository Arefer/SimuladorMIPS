//
// Created by kevin on 19-05-19.
//

#ifndef LAB3_INSTR_MEM_H
#define LAB3_INSTR_MEM_H

struct instruction{
    int line;
    char* address;  // 32 Bits
    char* name;
    char type;
    char* rs;  // 5 Bits
    char* rt;  // 5 Bits
    char* rd;  // Tipo R, 5 Bits
    int immediate;  // Tipo I
    char* j_address;  // Address de instruccion "j" y immediate de instruccion "beq"
    char* label;
};


typedef struct instruction Instruction;
Instruction *init_instr();
void print_instr(void* instr);
void free_instr(void* instruction);
#endif //LAB3_INSTR_MEM_H
