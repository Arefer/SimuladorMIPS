//
// Created by kevin on 20-05-19.
//

#include "register.h"
#include "../Auxiliar/linked_list.h"
#include "ram.h"
#include "instr_mem.h"

#ifndef LAB3_CPU_H
#define LAB3_CPU_H

typedef enum {NO, YES} ACTIVE_HAZARD;
// Etapas de pipeline clásico, a excepción de EX, que se divide en dos.
// EX1 es donde opera la ALU principal.
// EX2 es donde opera la ALU que calcula la direccion de un salto
enum stages{IF, ID, EX1, EX2, MEM, WB};

struct cpu{
    int cc;                       // Clock count
    char* PC;                     // Program Counter
    List* instr_memory;
    Register** reg_file;
    int jump_alu_enabled;
    Ram* ram_memory;
    Instruction** executing;      // 6 Etapas de Pipeline
    int pending_write;            // Numero del registro pendiente por escribir
    ACTIVE_HAZARD true_dependency;
    Instruction* NOP;
    int bubbles;
};
typedef struct cpu CPU;

CPU* init_cpu();
void add_to_pipeline(CPU* cpu, Instruction* instr, int stage);
void instruction_fetch(CPU* CPU);
int instruction_decode(CPU* CPU);
void ex1(CPU* CPU);
int ex2(CPU* CPU);
void mem(CPU* CPU);
int write_back(CPU* CPU);
int are_there_six_nops(CPU* cpu);
void run(CPU* CPU);
void print_pipeline(CPU* cpu, int cell_size);
void print_instr_mem(CPU* cpu);
void free_cpu(CPU* cpu);

#endif //LAB3_CPU_H