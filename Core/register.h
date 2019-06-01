//
// Created by kevin on 19-05-19.
//

#ifndef LAB3_REGISTER_H
#define LAB3_REGISTER_H

struct regist{
    char* name;
    char* number; // 32 Bits
    char* data;  // 32 Bits
};
typedef struct regist Register;

struct reg_file{
    char* readRegister1;  // 5 Bits
    char* readRegister2;  // 5 Bits
    char* readData1;
    char* readData2;
    Register** registers;
};
typedef struct reg_file Reg_file;



Register* init_register(char *name, char *number, char *data);
Reg_file* init_reg_file();
Register* searchRegister(Reg_file* reg_file, char* name);
Register* search_reg_by_number(Reg_file* reg_file, char* number);
void free_register(Register* reg);
void free_reg_file(Reg_file* reg_file);
#endif //LAB3_REGISTER_H
