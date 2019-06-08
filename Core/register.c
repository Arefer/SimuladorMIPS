//
// Created by kevin on 19-05-19.
//

#include "register.h"
#include "../Auxiliar/binary_ops.h"
#include <string.h>
#include <stdlib.h>

const char* zero = "$zero\0";
const char* at = "$at\0";
const char* v0 = "$v0\0";
const char* v1 = "$v1\0";
const char* a0 = "$a0\0";
const char* a1 = "$a1\0";
const char* a2 = "$a2\0";
const char* a3 = "$a3\0";
const char* t0 = "$t0\0";
const char* t1 = "$t1\0";
const char* t2 = "$t2\0";
const char* t3 = "$t3\0";
const char* t4 = "$t4\0";
const char* t5 = "$t5\0";
const char* t6 = "$t6\0";
const char* t7 = "$t7\0";
const char* s0 = "$s0\0";
const char* s1 = "$s1\0";
const char* s2 = "$s2\0";
const char* s3 = "$s3\0";
const char* s4 = "$s4\0";
const char* s5 = "$s5\0";
const char* s6 = "$s6\0";
const char* s7 = "$s7\0";
const char* t8 = "$t8\0";
const char* t9 = "$t9\0";
const char* k0 = "$k0\0";
const char* k1 = "$k1\0";
const char* gp = "$gp\0";
const char* sp = "$sp\0";
const char* fp = "$fp\0";
const char* ra = "$ra\0";

/**
 * Inicializa un Registro
 */
Register* init_register(const char *name, char *data) {
    Register* reg = (Register*)malloc(sizeof(Register));
    reg->name = name;
    reg->data = data;
    return reg;
}


/**
 * Inicializa el archivo de registros
 */
Register** init_reg_file() {
    Register** registers = (Register**)malloc(sizeof(Register*)*32);


    registers[$zero] = init_register(zero, decimal_to_binary(0, 32));
    registers[$at] = init_register(at, decimal_to_binary(0, 32));
    registers[$v0] = init_register(v0, decimal_to_binary(0, 32));
    registers[$v1] = init_register(v1, decimal_to_binary(0, 32));
    registers[$a0] = init_register(a0, decimal_to_binary(0, 32));
    registers[$a1] = init_register(a1, decimal_to_binary(0, 32));
    registers[$a2] = init_register(a2, decimal_to_binary(0, 32));
    registers[$a3] = init_register(a3, decimal_to_binary(0, 32));
    registers[$t0] = init_register(t0, decimal_to_binary(0, 32));
    registers[$t1] = init_register(t1, decimal_to_binary(0, 32));
    registers[$t2] = init_register(t2, decimal_to_binary(0, 32));
    registers[$t3] = init_register(t3, decimal_to_binary(0, 32));
    registers[$t4] = init_register(t4, decimal_to_binary(0, 32));
    registers[$t5] = init_register(t5, decimal_to_binary(0, 32));
    registers[$t6] = init_register(t6, decimal_to_binary(0, 32));
    registers[$t7] = init_register(t7, decimal_to_binary(0, 32));
    registers[$s0] = init_register(s0, decimal_to_binary(0, 32));
    registers[$s1] = init_register(s1, decimal_to_binary(0, 32));
    registers[$s2] = init_register(s2, decimal_to_binary(0, 32));
    registers[$s3] = init_register(s3, decimal_to_binary(0, 32));
    registers[$s4] = init_register(s4, decimal_to_binary(0, 32));
    registers[$s5] = init_register(s5, decimal_to_binary(0, 32));
    registers[$s6] = init_register(s6, decimal_to_binary(0, 32));
    registers[$s7] = init_register(s7, decimal_to_binary(0, 32));
    registers[$t8] = init_register(t8, decimal_to_binary(0, 32));
    registers[$t9] = init_register(t9, decimal_to_binary(0, 32));
    registers[$k0] = init_register(k0, decimal_to_binary(0, 32));
    registers[$k1] = init_register(k1, decimal_to_binary(0, 32));
    registers[$gp] = init_register(gp, decimal_to_binary(0, 32));
    registers[$sp] = init_register(sp, decimal_to_binary(10000, 32));
    registers[$fp] = init_register(fp, decimal_to_binary(0, 32));
    registers[$ra] = init_register(ra, decimal_to_binary(0, 32));

    return registers;

}

/*
 * Busca un registro segun su nombre. Retorna su indice.
 */
int search_register(Register** reg_file, char* name){
    int i = 0;
    while (i < 32){
        if (strcmp(reg_file[i]->name, name) == 0){
            return i;
        }
        i++;
    }
    return 0;
}

/*
 * Libera la memoria de un registro (Register)
 */
void free_register(Register* reg){
    free(reg->data);
    free(reg);
}

/*
 * Libera la memoria del archivo de registros
*/
void free_reg_file(Register** reg_file){
    int i = 0;
    while (i < 32){
        free_register(reg_file[i]);
        i++;
    }
    free(reg_file);
}
