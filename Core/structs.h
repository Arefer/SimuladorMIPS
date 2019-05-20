//
// Created by kevin on 18-05-19.
//

#ifndef LAB3_STRUCTS_H
#define LAB3_STRUCTS_H

#include "../Auxiliar/linked_list.h"

struct instruction{
    int line;
    char* address;  // 32 Bits
    char* name;
    char type;
    char* rs;  // 5 Bits
    char* rt;  // 5 Bits
    char* rd;  // Tipo R, 5 Bits
    int immediate;  // Tipo I
    char* j_address;  // Address de instruccion "j" y immediate de instruccion "beq"
    char* label;
};
typedef struct instruction Instruction;

struct mem_data{
    char* address;  // 32 Bits
    char* data;  // 32 Bits
};
typedef struct mem_data Mem_data;

struct regist{
    char* name;
    char* number; // 32 Bits
    char* data;  // 32 Bits
};
typedef struct regist Register;

struct reg_file{
    char* readRegister1;  // 5 Bits
    char* readRegister2;  // 5 Bits
    char* readData1;  // 32 Bits
    char* readData2;  // 32 Bits
    Register** registers;
};
typedef struct reg_file Reg_file;

struct controlUnit{
    int regDst;  // Puede fallar
    int jump;
    int branch;
    int memRead;
    int memtoReg;
    int ALUOp;
    int memWrite;
    int ALUSrc;  // Puede fallar
    int regWrite;  // Puede fallar
};
typedef struct controlUnit ControlUnit;

struct cpu{
    int cc;  // Clock count
    char* PC;  // Program Counter
    Reg_file* reg_file;
    List* instr_memory;
    List* ram_memory;
    List* executing;  // 6 Etapas de Pipeline
};
typedef struct cpu Cpu;

struct hazard{

};
typedef struct hazard Hazard;
#endif //LAB3_STRUCTS_H
