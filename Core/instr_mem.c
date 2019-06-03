//
// Created by kevin on 19-05-19.
//

#include "instr_mem.h"


// Definicion de nombres de las instrucciones
const char* NOP = "NOP\0";
const char* ADD = "add\0";
const char* SUB = "sub\0";
const char* ADDI = "addi\0";
const char* SUBI = "subi\0";
const char* BEQ = "beq\0";
const char* BNE = "bne\0";
const char* J = "j\0";
const char* LW = "lw\0";
const char* SW = "sw\0";
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

/*
 * Inicializa una instruccion NOP
 */
Instruction* init_nop_instr(){
    Instruction* nop = init_instr();
    nop->name = NOP;
    return nop;
}

/*
 * Busca una instruccion por su etiqueta.
 */
Instruction* search_by_label(List* instr_mem, char* label){
    Node* cursor = instr_mem->first;
    while (cursor != NULL){
        Instruction* instr = (Instruction*)cursor->data;
        if (strcmp(instr->label, label) == 0)
            return instr;
        cursor = cursor->next;
    }
    return NULL;
}

/**
 * Imprime por pantalla los campos de una instruccion
 */
void print_instr(void* instruction){
    Instruction* instr = (Instruction*) instruction;
    printf("Line: %d ", instr->line);
    if (instr->type == 'R'){
        // printf("Label: %s", instr->label);
        // printf(" Address: %s", instr->address);
        // printf(" Nombre: %s", instr->name);
        // printf(" rs: |%s|", instr->rs);
        // printf(" rt: |%s|", instr->rt);
        // printf(" rd: |%s|", instr->rd);
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
    if (instr->address != NULL)
        free(instr->address);
    // free(instr->name);
    if (instr->rs != NULL)
        free(instr->rs);
    if (instr->rt != NULL)
        free(instr->rt);
    if (instr->rd != NULL)
        free(instr->rd);
    if (instr->j_address != NULL)
        free(instr->j_address);
    if (instr->label != NULL)
        free(instr->label);
    free(instr);
}
