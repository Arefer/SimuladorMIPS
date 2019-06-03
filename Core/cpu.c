//
// Created by kevin on 20-05-19.
//

#include "cpu.h"
#include "../Auxiliar/binary_ops.h"
#include "register.h"
#include "instr_mem.h"
#include "ram.h"
#include "../Auxiliar/custom_string_ops.h"
#include <stdlib.h>
#include <string.h>

/*
 * Inicia un procesador con PC en 0 y archivo de registros, ram, memoria de instrucciones vacias
 */
CPU* init_cpu(){
    CPU* cpu = (CPU*) malloc(sizeof(CPU));
    cpu->cc = 0;
    cpu->PC = decimal_to_binary(0, 32);
    cpu->reg_file = init_reg_file();
    cpu->instr_memory = init_list();
    cpu->ram_memory = init_ram_mem();
    cpu->NOP = init_nop_instr();
    cpu->executing = (Instruction**)malloc(sizeof(Instruction*)*6);
    cpu->executing[IF] = cpu->NOP;
    cpu->executing[ID] = cpu->NOP;
    cpu->executing[EX1] = cpu->NOP;
    cpu->executing[EX2] = cpu->NOP;
    cpu->executing[MEM] = cpu->NOP;
    cpu->executing[WB] = cpu->NOP;
    cpu->hazard = NULL;
    return cpu;
}

/*
 * Añade una instruccion al pipeline en la etapa indicada.
 * Desplaza hacia la derecha las instrucciones a partir de la etapa indicada.
 */
void add_to_pipeline(CPU* p, Instruction* instr, const int stage){
    int i = 5;
    while (i > stage){
        p->executing[i] = p->executing[i-1];
        i--;
    }
    p->executing[i] = instr;
}


/*
 * Simula la etapa de instruction fetch (IF).
 * Busca la instruccion apuntada por el PC en la memoria de instrucciones
 */
void instruction_fetch(CPU* CPU){
    // Si no hay riesgo de datos activo
    if (CPU->true_dependency == NO) {
        // Buscar la instruccion
        Instruction *instr = (Instruction *) find(CPU->instr_memory, CPU->PC);
        add_to_pipeline(CPU, instr, IF);
        // PC = PC+4
        char* PC = two_binary_sum(CPU->PC, decimal_to_binary(4, 32));
        free(CPU->PC);
        CPU->PC = PC;
    }
}

/*
 * Simula las etapas de instruction decode (ID) y write back (WB).
 * Se mezclan estas etapas para asegurar que se escriba primero antes de acceder al archivo de registros.
 */
void write_back_and_decode(CPU* CPU){
    // Write Back
    Instruction* wb_instr = CPU->executing[WB];
    Register* rs = searchRegister(CPU->reg_file, wb_instr->rs);
    Register* rt = searchRegister(CPU->reg_file, wb_instr->rt);
    Register* rd = searchRegister(CPU->reg_file, wb_instr->rd);
    if (strcmp(wb_instr->name, LW) == 0){
        free(rt->data);
        char* read_data = (char*)pop_from_list(CPU->ram_memory->read_data);
        strcpy(rt->data, read_data);
        free(read_data);
        if (strcmp(wb_instr->rt, CPU->hazard) == 0)
            CPU->true_dependency = NO;
    } else if (strcmp(wb_instr->name, SUB) == 0){
        // $rd = $rs - $rt
        free(rd->data);
        rd->data = two_binary_rest(rs->data, rt->data);
        if (strcmp(wb_instr->rd, CPU->hazard) == 0)
            CPU->true_dependency = NO;
    } else if ((strcmp(wb_instr->name, ADD) == 0)){
        // $rd = $rs + $rt
        free(rd->data);
        rd->data = two_binary_sum(rs->data, rt->data);
        if (strcmp(wb_instr->rd, CPU->hazard) == 0)
            CPU->true_dependency = NO;
    }else if ((strcmp(wb_instr->name, ADDI) == 0) || (strcmp(wb_instr->name, SUBI) == 0)){
        // $rs + Immediate = $rt
        int decimal_rs = binary_to_decimal(rs->data);
        free(rt->data);
        rt->data = decimal_to_binary(decimal_rs + wb_instr->immediate, 32);
        if (strcmp(wb_instr->rt, CPU->hazard) == 0)
            CPU->true_dependency = NO;
    }


    // Como en este simulador, el decode se hace al momento de leer las instrucciones, en
    // esta funcion solo identificaremos si existe un data hazard
    Instruction* id_instr = CPU->executing[ID];
    if ((strcmp(id_instr->name, ADD) == 0) || (strcmp(id_instr->name, SUB) == 0)) {
        // Si el campo rt o rs de la instruccion en ID es igual a hazard, entonces hay un riesgo de datos
        if ((strcmp(id_instr->rs, CPU->hazard) == 0) || (strcmp(id_instr->rt, CPU->hazard) == 0)) {
            CPU->true_dependency = YES;
        } else {  // Si no es el caso, indicamos que el registro rd queda pendiente por escribir
            CPU->hazard = id_instr->rd;
        }

    } else if ((strcmp(id_instr->name, BEQ) == 0) || (strcmp(id_instr->name, BNE) == 0)){
        // Si el campo rt o rs de la instruccion en ID es igual a hazard, entonces hay un riesgo de datos
        if ((strcmp(id_instr->rs, CPU->hazard) == 0) || (strcmp(id_instr->rt, CPU->hazard) == 0))
            CPU->true_dependency = YES;

    } else if ((strcmp(id_instr->name, ADDI) == 0) || (strcmp(id_instr->name, SUBI) == 0)
            || (strcmp(id_instr->name, LW) == 0) || (strcmp(id_instr->name, SW) == 0)) {
        // Si el campo rs de la instruccion en ID es igual a hazard, entonces hay un riesgo de datos
        if (strcmp(id_instr->rs, CPU->hazard) == 0){
            CPU->true_dependency = YES;
        } else {  // Si no es el caso, indicamos que el registro rt queda pendiente por escribir
            CPU->hazard = id_instr->rt;
        }
    }

    // Añadir burbuja en caso de data hazard
    if (CPU->true_dependency == YES) add_to_pipeline(CPU, CPU->NOP, EX1);
}

/*
 * Simula la etapa de operacion de la ALU principal (EX1)
 */
void ex1(CPU* CPU){
    Instruction* instr = CPU->executing[EX1];
    if (strcmp(instr->name, LW) == 0) {
        // $rs + Immediate para calcular la direccion del dato a cargar
        Register* rs = searchRegister(CPU->reg_file, instr->rs);
        int decimal_rs = binary_to_decimal(rs->data);
        int decimal_addr = decimal_rs + instr->immediate;
        CPU->ram_memory->addr = decimal_to_binary(decimal_addr, 32);
    } else if (strcmp(instr->name, SW) == 0){
        // $rs + Immediate para calcular la direccion del dato a escribir
        Register* rs = searchRegister(CPU->reg_file, instr->rs);
        int decimal_rs = binary_to_decimal(rs->data);
        int decimal_addr = decimal_rs + instr->immediate;
        CPU->ram_memory->addr = decimal_to_binary(decimal_addr, 32);
        // Indicamos que el dato a escribir es el dato almacenado en el rt de la instruccion
        Register* rt = searchRegister(CPU->reg_file, instr->rt);
        CPU->ram_memory->write_data = rt->data;
    } else if (strcmp(instr->name, BEQ) == 0){
        // Comparamos los campos rs con rt de la instruccion
        Register* rs = searchRegister(CPU->reg_file, instr->rs);
        Register* rt = searchRegister(CPU->reg_file, instr->rt);
        // Si son iguales, activamos la jump_alu (simula un multiplexor)
        if (strcmp(rs->data, rt->data) == 0){
            CPU->j_alu->enabled = 1;
            // Como se hizo el branch, es necesario vaciar las instrucciones erroneamente leidas
            CPU->executing[IF] = CPU->NOP;
            CPU->executing[ID] = CPU->NOP;
            // Ademas, como la instruccion que le precede esta en MEM, en 2 ciclos mas es imposible un data hazard
            CPU->true_dependency = NO;
        }
    } else if (strcmp(instr->name, BNE) == 0){
        // Comparamos los campos rs con rt de la instruccion
        Register* rs = searchRegister(CPU->reg_file, instr->rs);
        Register* rt = searchRegister(CPU->reg_file, instr->rt);
        // Si no son iguales, activamos la jump_alu (simula un multiplexor)
        if (strcmp(rs->data, rt->data) != 0){
            CPU->j_alu->enabled = 1;
            // Como se hizo el branch, es necesario vaciar las instrucciones erroneamente leidas
            CPU->executing[IF] = CPU->NOP;
            CPU->executing[ID] = CPU->NOP;
            // Ademas, como la instruccion que le precede esta en MEM, en 2 ciclos mas es imposible un data hazard
            CPU->true_dependency = NO;
        }
    }
}

/*
 * Simula la etapa de operacion de la ALU que calcula direcciones de los saltos (EX2)
 */
void ex2(CPU* CPU){
    if (CPU->j_alu->enabled == 1){
        Instruction* instr = CPU->executing[EX2];
        // Instruccion j
        if (strcmp(instr->name, J) == 0){
            // Si el immediate de la instruccion es un numero
            if (is_number(instr->j_address) == 1){
                // sll 2
                int decimal_26 = binary_to_decimal(instr->j_address);
                int decimal_28 = decimal_26*4;
                char* binary_28 = decimal_to_binary(decimal_28, 28);
                // Concatenamos los 4 bits iniciales del PC al principio de la nueva direccion (binary_32)
                char* binary_32 = (char*)malloc(sizeof(char)*33);
                memcpy(binary_32, CPU->PC, 4);
                // Los siguientes 28 bits son los correspondientes al sll 2 del immediate
                memcpy(binary_32+4, binary_28, strlen(binary_28)+1);
                binary_32[32] = '\0';

                free(binary_28);
                free(CPU->PC);
                CPU->PC = (char*)malloc(sizeof(char)*33);
                // Actualizamos el valor del PC
                strcpy(CPU->PC, binary_32);
                free(binary_32);
            } else {  // Si es una etiqueta
                // Buscamos la instruccion con dicha etiqueta en la memoria de instrucciones
                Instruction* j_instr = search_by_label(CPU->instr_memory, instr->label);
                // Actualizamos el valor del PC por la direccion de la instruccion con la etiqueta
                free(CPU->PC);
                CPU->PC = (char*)malloc(sizeof(char)*33);
                strcpy(CPU->PC, j_instr->address);
            }
        // Instrucciones beq y bne
        } else if (strcmp(instr->name, BEQ) == 0 || strcmp(instr->name, BNE) == 0){
            // Si el immediate de la instruccion es un numero
            if (is_number(instr->j_address) == 1){
                // Se lo sumamos al PC
                char* imm_32 = sign_extend(instr->j_address, 32);
                char* current_PC = (char*)malloc(sizeof(char)*33);
                strcpy(current_PC, CPU->PC);
                free(CPU->PC);
                CPU->PC = two_binary_sum(imm_32, current_PC);
                free(imm_32);
                free(current_PC);
            } else {  // Si es una etiqueta
                // Buscamos la instruccion con dicha etiqueta en la memoria de instrucciones
                Instruction* j_instr = search_by_label(CPU->instr_memory, instr->label);
                // Actualizamos el valor del PC por la direccion de la instruccion con la etiqueta
                free(CPU->PC);
                CPU->PC = (char*)malloc(sizeof(char)*33);
                strcpy(CPU->PC, j_instr->address);
            }
        }
    }
}

/*
 * Simula la etapa de lectura y escritura de memoria (MEM)
 */
void mem(CPU* CPU){
    Instruction* instr = CPU->executing[MEM];
    if (strcmp(instr->name, LW) == 0){
        read_from_ram(CPU->ram_memory);
    } else if (strcmp(instr->name, SW) == 0){
        write_to_ram(CPU->ram_memory);
    }
}
/*
 * Imprime la memoria de instrucciones
 */
void print_instr_mem(CPU* cpu){
    iterate(cpu->instr_memory, print_instr);
}

/*
 * Libera la memoria utilizada por el procesador
 */
void free_cpu(CPU* cpu){
    free(cpu->PC);
    free_reg_file(cpu->reg_file);
    free(cpu->executing);
    free_ram_mem(cpu->ram_memory);
    free_list_and_nodes(cpu->instr_memory, free_instr);
    free_instr(cpu->NOP);
    free(cpu);
}