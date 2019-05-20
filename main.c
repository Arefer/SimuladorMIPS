#include <stdio.h>
#include "Core/io.h"
#include "Core/structs.h"
#include "Core/register.h"
#include "Core/instr_mem.h"
#include "Core/io.h"
#include "Core/cpu.h"

int main() {
    Cpu* cpu = init_cpu();
    read_source_code("test", cpu);
    print_instr_mem(cpu);
    free_cpu(cpu);
    return 0;
}