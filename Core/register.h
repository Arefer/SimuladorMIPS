//
// Created by kevin on 19-05-19.
//

#ifndef LAB3_REGISTER_H
#define LAB3_REGISTER_H
enum Registers{
    $zero,
    $at,
    $v0,
    $v1,
    $a0,
    $a1,
    $a2,
    $a3,
    $t0,
    $t1,
    $t2,
    $t3,
    $t4,
    $t5,
    $t6,
    $t7,
    $s0,
    $s1,
    $s2,
    $s3,
    $s4,
    $s5,
    $s6,
    $s7,
    $t8,
    $t9,
    $k0,
    $k1,
    $gp,
    $sp,
    $fp,
    $ra
};

extern const char* zero;
extern const char* at;
extern const char* v0;
extern const char* v1;
extern const char* a0;
extern const char* a1;
extern const char* a2;
extern const char* a3;
extern const char* t0;
extern const char* t1;
extern const char* t2;
extern const char* t3;
extern const char* t4;
extern const char* t5;
extern const char* t6;
extern const char* t7;
extern const char* s0;
extern const char* s1;
extern const char* s2;
extern const char* s3;
extern const char* s4;
extern const char* s5;
extern const char* s6;
extern const char* s7;
extern const char* t8;
extern const char* t9;
extern const char* k0;
extern const char* k1;
extern const char* gp;
extern const char* sp;
extern const char* fp;
extern const char* ra;

struct regist{
    const char* name;
    char* data;  // 32 Bits
};
typedef struct regist Register;




Register* init_register(const char *name, char *data);
Register** init_reg_file();
int search_register(Register** reg_file, char* name);
void free_register(Register* reg);
void free_reg_file(Register** reg_file);
#endif //LAB3_REGISTER_H
