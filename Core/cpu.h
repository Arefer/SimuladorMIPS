//
// Created by kevin on 20-05-19.
//

#include "register.h"
#include "../Auxiliar/linked_list.h"
#include "ram.h"
#include "instr_mem.h"

#ifndef LAB3_CPU_H
#define LAB3_CPU_H

// Etapas de pipeline clásico, a excepción de EX, que se divide en dos.
// EX1 es donde opera la ALU principal.
// EX2 es donde opera la ALU que calcula la direccion de un salto
enum stages{IF, ID, EX1, EX2, MEM, WB};
typedef enum {NO, YES} ACTIVE_HAZARD;

struct main_alu{
    char* op1;
    char* op2;
    int zero;
};
typedef struct main_alu Main_alu;

struct jump_alu{
    char* op1;
    char* op2;
    int enabled;
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
    Instruction** executing;  // 6 Etapas de Pipeline
    char* hazard;  // Nombre del registro pendiente por escribir
    ACTIVE_HAZARD true_dependency;
    Instruction* NOP;
};
typedef struct cpu CPU;

CPU* init_cpu();
void add_to_pipeline(CPU* p, Instruction* instr, int stage);
void instruction_fetch(CPU* CPU);
void write_back_and_decode(CPU* CPU);
void ex1(CPU* CPU);
void ex2(CPU* CPU);
void print_instr_mem(CPU* cpu);
void free_cpu(CPU* cpu);

#endif //LAB3_CPU_H