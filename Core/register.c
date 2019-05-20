//
// Created by kevin on 19-05-19.
//

#include "register.h"
#include "../Auxiliar/binary_ops.h"
#include <string.h>
#include <stdlib.h>



/**
 * Inicializa un Registro
 */
Register* init_register(char *name, char *number, char *data) {
    Register* reg = (Register*)malloc(sizeof(Register));
    reg->name = name;
    reg->number = number;
    reg->data = data;
    return reg;
}


/**
 * Inicializa el archivo de registros
 */
Reg_file *init_reg_file() {
    Reg_file* reg_file = (Reg_file*)malloc(sizeof(Reg_file));
    reg_file->readRegister1 = NULL;
    reg_file->readRegister2 = NULL;
    reg_file->readData1 = NULL;
    reg_file->readData2 = NULL;
    reg_file->registers = (Register**)malloc(sizeof(Register*)*32);

    char* reg_zero = (char*)malloc(sizeof(char)*6);
    strcpy(reg_zero, "$zero\0");

    char* reg_at = (char*)malloc(sizeof(char)*4);
    strcpy(reg_at, "$at\0");

    char* reg_v0 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_v0, "$v0\0");

    char* reg_v1 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_v1, "$v1\0");

    char* reg_a0 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_a0, "$a0\0");

    char* reg_a1 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_a1, "$a1\0");

    char* reg_a2 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_a2, "$a2\0");

    char* reg_a3 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_a3, "$a3\0");

    char* reg_t0 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_t0, "$t0\0");

    char* reg_t1 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_t1, "$t1\0");

    char* reg_t2 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_t2, "$t2\0");

    char* reg_t3 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_t3, "$t3\0");

    char* reg_t4 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_t4, "$t4\0");

    char* reg_t5 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_t5, "$t5\0");

    char* reg_t6 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_t6, "$t6\0");

    char* reg_t7 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_t7, "$t7\0");

    char* reg_s0 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_s0, "$s0\0");

    char* reg_s1 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_s1, "$s1\0");

    char* reg_s2 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_s2, "$s2\0");

    char* reg_s3 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_s3, "$s3\0");

    char* reg_s4 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_s4, "$s4\0");

    char* reg_s5 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_s5, "$s5\0");

    char* reg_s6 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_s6, "$s6\0");

    char* reg_s7 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_s7, "$s7\0");

    char* reg_t8 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_t8, "$t8\0");

    char* reg_t9 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_t9, "$t9\0");

    char* reg_k0 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_k0, "$k0\0");

    char* reg_k1 = (char*)malloc(sizeof(char)*4);
    strcpy(reg_k1, "$k1\0");

    char* reg_gp = (char*)malloc(sizeof(char)*4);
    strcpy(reg_gp, "$gp\0");

    char* reg_sp = (char*)malloc(sizeof(char)*4);
    strcpy(reg_sp, "$sp\0");

    char* reg_fp = (char*)malloc(sizeof(char)*4);
    strcpy(reg_fp, "$fp\0");

    char* reg_ra = (char*)malloc(sizeof(char)*4);
    strcpy(reg_ra, "$ra\0");


    reg_file->registers[0] = init_register(reg_zero, decimal_to_binary(0, 32),
                                           decimal_to_binary(0, 32));

    reg_file->registers[1] = init_register(reg_at, decimal_to_binary(1, 32),
                                           decimal_to_binary(0, 32));

    reg_file->registers[2] = init_register(reg_v0, decimal_to_binary(2, 32),
                                           decimal_to_binary(0, 32));

    reg_file->registers[3] = init_register(reg_v1, decimal_to_binary(3, 32),
                                           decimal_to_binary(0, 32));

    reg_file->registers[4] = init_register(reg_a0, decimal_to_binary(4, 32),
                                           decimal_to_binary(0, 32));

    reg_file->registers[5] = init_register(reg_a1, decimal_to_binary(5, 32),
                                           decimal_to_binary(0, 32));

    reg_file->registers[6] = init_register(reg_a2, decimal_to_binary(6, 32),
                                           decimal_to_binary(0, 32));

    reg_file->registers[7] = init_register(reg_a3, decimal_to_binary(7, 32),
                                           decimal_to_binary(0, 32));

    reg_file->registers[8] = init_register(reg_t0, decimal_to_binary(8, 32),
                                           decimal_to_binary(0, 32));

    reg_file->registers[9] = init_register(reg_t1, decimal_to_binary(9, 32),
                                           decimal_to_binary(0, 32));

    reg_file->registers[10] = init_register(reg_t2, decimal_to_binary(10, 32),
                                            decimal_to_binary(0, 32));

    reg_file->registers[11] = init_register(reg_t3, decimal_to_binary(11, 32),
                                            decimal_to_binary(0, 32));

    reg_file->registers[12] = init_register(reg_t4, decimal_to_binary(12, 32),
                                            decimal_to_binary(0, 32));

    reg_file->registers[13] = init_register(reg_t5, decimal_to_binary(13, 32),
                                            decimal_to_binary(0, 32));

    reg_file->registers[14] = init_register(reg_t6, decimal_to_binary(14, 32),
                                            decimal_to_binary(0, 32));

    reg_file->registers[15] = init_register(reg_t7, decimal_to_binary(15, 32),
                                            decimal_to_binary(0, 32));

    reg_file->registers[16] = init_register(reg_s0, decimal_to_binary(16, 32),
                                            decimal_to_binary(0, 32));

    reg_file->registers[17] = init_register(reg_s1, decimal_to_binary(17, 32),
                                            decimal_to_binary(0, 32));

    reg_file->registers[18] = init_register(reg_s2, decimal_to_binary(18, 32),
                                            decimal_to_binary(0, 32));

    reg_file->registers[19] = init_register(reg_s3, decimal_to_binary(19, 32),
                                            decimal_to_binary(0, 32));

    reg_file->registers[20] = init_register(reg_s4, decimal_to_binary(20, 32),
                                            decimal_to_binary(0, 32));

    reg_file->registers[21] = init_register(reg_s5, decimal_to_binary(21, 32),
                                            decimal_to_binary(0, 32));

    reg_file->registers[22] = init_register(reg_s6, decimal_to_binary(22, 32),
                                            decimal_to_binary(0, 32));

    reg_file->registers[23] = init_register(reg_s7, decimal_to_binary(23, 32),
                                            decimal_to_binary(0, 32));

    reg_file->registers[24] = init_register(reg_t8, decimal_to_binary(24, 32),
                                            decimal_to_binary(0, 32));

    reg_file->registers[25] = init_register(reg_t9, decimal_to_binary(25, 32),
                                            decimal_to_binary(0, 32));

    reg_file->registers[26] = init_register(reg_k0, decimal_to_binary(26, 32),
                                            decimal_to_binary(0, 32));

    reg_file->registers[27] = init_register(reg_k1, decimal_to_binary(27, 32),
                                            decimal_to_binary(0, 32));

    reg_file->registers[28] = init_register(reg_gp, decimal_to_binary(28, 32),
                                            decimal_to_binary(0, 32));

    reg_file->registers[29] = init_register(reg_sp, decimal_to_binary(29, 32),
                                            decimal_to_binary(0, 32));

    reg_file->registers[30] = init_register(reg_fp, decimal_to_binary(30, 32),
                                            decimal_to_binary(0, 32));

    reg_file->registers[31] = init_register(reg_ra, decimal_to_binary(31, 32),
                                            decimal_to_binary(0, 32));

    return reg_file;

}

/**
 * Busca un registro segun su nombre
 */
Register* searchRegister(Reg_file* reg_file, char* name){
    int i = 0;
    while (i < 32){
        if (strcmp(reg_file->registers[i]->name, name) == 0){
            return reg_file->registers[i];
        }
        i++;
    }
    return NULL;
}


/**
 * Busca un registro por su numero
 */
Register* search_reg_by_number(Reg_file* reg_file, char* number){
    int i = 0;
    while (i < 32){
        Register* reg = reg_file->registers[i];
        if (strcmp(reg->number,number) == 0)
            return reg;
        i++;
    }
    return NULL;
}

/*
 * Libera la memoria de un registro (Register)
 */
void free_register(Register* reg){
    free(reg->name);
    free(reg->number);
    free(reg->data);
    free(reg);
}

/*
 * Libera la memoria del archivo de registros
*/
void free_reg_file(Reg_file* reg_file){
    int i = 0;
    while (i < 32){
        free_register(reg_file->registers[i]);
        i++;
    }
    free(reg_file->registers);
    free(reg_file);
}