//
// Created by kevin on 19-05-19.
//
#include "structs.h"
#ifndef LAB3_REGISTER_H
#define LAB3_REGISTER_H
Register* init_register(char *name, char *number, char *data);
Reg_file* init_reg_file();
Register* searchRegister(Reg_file* reg_file, char* name);
Register* search_reg_by_number(Reg_file* reg_file, char* number);
void free_register(Register* reg);
void free_reg_file(Reg_file* reg_file);
#endif //LAB3_REGISTER_H
