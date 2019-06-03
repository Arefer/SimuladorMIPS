#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Core/io.h"
#include "Core/register.h"
#include "Core/instr_mem.h"
#include "Core/io.h"
#include "Core/cpu.h"
#include "Auxiliar/binary_ops.h"


int main() {
    char* test = (char*)malloc(sizeof(5));
    strcpy(test, "1111\0");
    char* binary_32 = (char*)malloc(sizeof(char)*33);
    char* binary_28 = decimal_to_binary(10, 28);
    memcpy(binary_32, test, 4);
    memcpy(binary_32+4, binary_28, strlen(binary_28)+1);
    binary_32[32] = '\0';
    free(test);
    free(binary_28);
    /*CPU* cpu = init_cpu();
    read_source_code("test", cpu);
    print_instr_mem(cpu);
    free_cpu(cpu);*/
    return 0;
}