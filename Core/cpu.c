//
// Created by kevin on 20-05-19.
//

#include "cpu.h"
#include "../Auxiliar/binary_ops.h"
#include "register.h"
#include "instr_mem.h"
#include "ram.h"
#include <stdlib.h>

/*
 * Inicia un procesador con PC en 0 y archivo de registros, ram, memoria de instrucciones vacias
 */
Cpu* init_cpu(){
    Cpu* cpu = (Cpu*) malloc(sizeof(Cpu));
    cpu->cc = 0;
    cpu->PC = decimal_to_binary(0, 32);
    cpu->reg_file = init_reg_file();
    cpu->instr_memory = init_list();
    cpu->ram_memory = init_list();
    cpu->executing = init_list();
    return cpu;
}

/*
 * Imprime la memoria de instrucciones
 */
void print_instr_mem(Cpu* cpu){
    iterate(cpu->instr_memory, print_instr);
}

/*
 * Libera la memoria utilizada por el procesador
 */
void free_cpu(Cpu* cpu){
    free(cpu->PC);
    free_reg_file(cpu->reg_file);
    free_list(cpu->executing);
    free_list_and_nodes(cpu->ram_memory, free_mem_data);
    free_list_and_nodes(cpu->instr_memory, free_instr);
    free(cpu);
}