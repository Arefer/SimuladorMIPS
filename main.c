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
    CPU* cpu = init_cpu();
    read_source_code("1.in", cpu);
    print_instr_mem(cpu);
    run(cpu);
    free_cpu(cpu);
    return 0;
}