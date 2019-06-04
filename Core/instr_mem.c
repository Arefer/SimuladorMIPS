//
// Created by kevin on 19-05-19.
//

#include "instr_mem.h"
#include "../Auxiliar/custom_string_ops.h"
#include "cpu.h"


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
    instr->rs = -1;
    instr->rt = -1;
    instr->rd = -1;
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
        if (instr->label != NULL && strcmp(instr->label, label) == 0)
            return instr;
        cursor = cursor->next;
    }
    return NULL;
}


/*
 * Imprime una instruccion, mostrando nombres de los registros, en vez de sus numeros.
 * No imprime saltos de linea
 */
void print_instr(Instruction* instr, Register** reg_file){
    Register* rs;
    Register* rt;
    Register* rd;
    // printf("Linea %d: ", instr->line);
    if (instr->type == 'R'){
        rs = reg_file[instr->rs];
        rt = reg_file[instr->rt];
        rd = reg_file[instr->rd];
        printf("%s %s, %s, %s", instr->name, rd->name, rs->name, rt->name);
    } else if (strcmp(instr->name, LW) == 0 || strcmp(instr->name, SW) == 0){
        rs = reg_file[instr->rs];
        rt = reg_file[instr->rt];
        printf("%s %s, %d(%s)", instr->name, rt->name, instr->immediate, rs->name);
    } else if (strcmp(instr->name, BNE) == 0 || strcmp(instr->name, BEQ) == 0){
        rs = reg_file[instr->rs];
        rt = reg_file[instr->rt];
        printf("%s %s, %s, %s", instr->name, rt->name, rs->name, instr->j_address);
    } else if (strcmp(instr->name, ADDI) == 0 || strcmp(instr->name, SUBI) == 0) {
        rs = reg_file[instr->rs];
        rt = reg_file[instr->rt];
        printf("%s %s, %s, %d", instr->name, rt->name, rs->name, instr->immediate);
    } else if (instr->type == 'J'){
        printf("%s %s", instr->name, instr->j_address);
    } else if (strcmp(instr->name, NOP) == 0){
        printf("%s", NOP);
    }
}


/*
 * Libera la memoria de una instruccion (Instruction)
*/
void free_instr(void* instruction){
    Instruction* instr = (Instruction*) instruction;
    if (instr->address != NULL)
        free(instr->address);
    if (instr->j_address != NULL)
        free(instr->j_address);
    if (instr->label != NULL)
        free(instr->label);
    free(instr);
}
