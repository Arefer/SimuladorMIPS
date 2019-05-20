//
// Created by kevin on 19-05-19.
//

#include "instr_mem.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * Inicializa una instruccion
 *
 */
Instruction *init_instr() {
    Instruction* instr = (Instruction*)malloc(sizeof(Instruction));
    instr->address = NULL;
    instr->name = NULL;
    instr->type = 0;
    instr->rs = NULL;
    instr->rt = NULL;
    instr->rd = NULL;
    instr->immediate = 0;
    instr->j_address = NULL;
    instr->label = NULL;
    return instr;
}


/**
 * Imprime por pantalla los campos de una instruccion
 */
void print_instr(void* instruction){
    Instruction* instr = (Instruction*) instruction;
    printf("Line: %d ", instr->line);
    if (instr->type == 'R'){
        printf("Label: %s", instr->label);
        printf(" Address: %s", instr->address);
        printf(" Nombre: %s", instr->name);
        printf(" rs: |%s|", instr->rs);
        printf(" rt: |%s|", instr->rt);
        printf(" rd: |%s|", instr->rd);
    } else if (instr->type == 'I'){
        printf("Label: %s", instr->label);
        printf(" Address: %s", instr->address);
        printf(" Nombre: %s", instr->name);
        printf(" rs: %s", instr->rs);
        printf(" rt: %s", instr->rt);
        printf(" Immediate: %d", instr->immediate);
    } else if (instr->type == 'J'){
        printf("Label: %s", instr->label);
        printf(" Address: %s", instr->address);
        printf(" Nombre: %s", instr->name);
        printf(" j_address: %s", instr->j_address);
    }
    printf("\n");
}

/*
 * Libera la memoria de una instruccion (Instruction)
*/
void free_instr(void* instruction){
    Instruction* instr = (Instruction*) instruction;
    free(instr->address);
    free(instr->name);
    free(instr->rs);
    free(instr->rt);
    free(instr->rd);
    free(instr->j_address);
    free(instr->label);
    free(instr);
}
