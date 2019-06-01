//
// Created by kevin on 20-05-19.
//

#include "register.h"
#include "../Auxiliar/linked_list.h"
#include "ram.h"

#ifndef LAB3_CPU_H
#define LAB3_CPU_H

struct main_alu{
    char* op1;
    char* op2;
    int zero;
};
typedef struct main_alu Main_alu;

struct jump_alu{
    char* op1;
    char* op2;
};
typedef struct jump_alu J_alu;

struct cpu{
    int cc;  // Clock count
    char* PC;  // Program Counter
    List* instr_memory;
    Reg_file* reg_file;
    Main_alu* main_alu;
    J_alu* j_alu;
    Ram* ram_memory;
    List* executing;  // 6 Etapas de Pipeline
};
typedef struct cpu Cpu;

Cpu* init_cpu();
void print_instr_mem(Cpu* cpu);
void free_cpu(Cpu* cpu);

#endif //LAB3_CPU_H