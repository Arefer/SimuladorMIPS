//
// Created by Kevin Arévalo on 20-05-19.
//

#include "cpu.h"
#include "../Auxiliar/binary_ops.h"
#include "register.h"
#include "instr_mem.h"
#include "ram.h"
#include "../Auxiliar/custom_string_ops.h"
#include <stdlib.h>
#include <string.h>

/**
 * Inicia un procesador con PC en 0 y memoria de instrucciones y ram vacias. El archivo de registros se inicializa con
 * los 32 registros correspondientes a la arquitectura MIPS de 32 bits, con sus datos en 0, a excepcion del $sp, que se
 * inicializa con un valor de 10000 (decimal).
 * @return CPU*
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
    cpu->pending_write = -1;
    cpu->bubbles = 0;
    return cpu;
}

/**
 * Añade una instruccion al pipeline en la etapa indicada.
 * Desplaza hacia la derecha las instrucciones a partir de la etapa indicada.
 * @param cpu: procesador que contiene al pipeline
 * @param instr: instruccion a agregar al pipeline
 * @param stage: Etapa del pipeline (IF, ID, EX1, EX2, MEM o WB)
 * @example Si el estado del pipeline es el siguiente {1, 2, 3, 4, 5, 6}, en donde los numeros representan a una
 * instruccion en IF, ID, EX1, EX2, MEM y WB respectivamente, y se ingresa una instrucción NOP en EX1, entonces el
 * estado del pipeline resulta {1, 2, NOP, 3, 4, 5}.
 */
void add_to_pipeline(CPU* cpu, Instruction* instr, const int stage){
    int i = 5;
    while (i > stage){
        cpu->executing[i] = cpu->executing[i-1];
        i--;
    }
    cpu->executing[i] = instr;
}


/**
 * Simula la etapa de instruction fetch (IF).
 * Busca la instruccion apuntada por el PC en la memoria de instrucciones y la añade a IF solamente si no existe un
 * data hazard no solucionado (indicado en el campo true_dependency del CPU).
 * Si no encuentra una instruccion, añade un NOP
 *
 * Además, si se añadio la instruccion, se actualiza el valor del PC. PC = PC + 4.
 * @param CPU
 */
void instruction_fetch(CPU* CPU){
    // Buscar la instruccion
    Instruction* instr = (Instruction*)find(CPU->instr_memory, CPU->PC);
    // Si no encuentro instruccion, decir que es un NOP
    if (instr == NULL) instr = CPU->NOP;
    // Hacer IF solamente si es que no hay data hazard
    if (CPU->true_dependency == NO){
        add_to_pipeline(CPU, instr, IF);
        // Aumentar el PC
        char* current_PC = (char*)malloc(sizeof(char)*33);
        strcpy(current_PC, CPU->PC);
        free(CPU->PC);
        CPU->PC = binary_sum(current_PC, 4, 32);
        free(current_PC);
    }
}

/**
 * Simula la etapa de instruction decode (ID).
 * Como el Decode en este simulador se hace al momento de leer el archivo con el código fuente, en esta
 * etapa sólo se identifica si es que existe un data hazard. Las instrucciones que pueden causar data hazard son add,
 * sub, addi, subi, lw, sw, beq y bne.
 *
 * Si se identifica un data hazard, se indica asignándole el valor YES al campo true_dependency del CPU.
 * En caso de no haber data hazard, se indica el número del registro que queda pendiente por escribir (puede ocasionar
 * un data hazard) en el campo pending_write del CPU.
 * @param CPU
 * @return 1 si se identifica un data hazard, 0 en caso contrario.
 */
int instruction_decode(CPU* CPU){
    Instruction* id_instr = CPU->executing[ID];
    int true_dependency_before;
    if (CPU->true_dependency == NO) true_dependency_before = 0;
    else true_dependency_before = 1;
    if ((strcmp(id_instr->name, ADD) == 0) || (strcmp(id_instr->name, SUB) == 0)) {
        // Si el campo rt o rs de la instruccion en ID es igual a pending_write, entonces hay un riesgo de datos
        if (id_instr->rs == CPU->pending_write || id_instr->rt == CPU->pending_write) {
            CPU->true_dependency = YES;
        } else {  // Si no es el caso, indicamos que el registro rd queda pendiente por escribir
            CPU->pending_write = id_instr->rd;
        }

    } else if ((strcmp(id_instr->name, BEQ) == 0) || (strcmp(id_instr->name, BNE) == 0)){
        // Si el campo rt o rs de la instruccion en ID es igual a pending_write, entonces hay un riesgo de datos
        if (id_instr->rs == CPU->pending_write || id_instr->rt == CPU->pending_write)
            CPU->true_dependency = YES;

    } else if ((strcmp(id_instr->name, ADDI) == 0) || (strcmp(id_instr->name, SUBI) == 0)
            || (strcmp(id_instr->name, LW) == 0) || (strcmp(id_instr->name, SW) == 0)) {
        // Si el campo rs de la instruccion en ID es igual a pending_write, entonces hay un riesgo de datos
        if (id_instr->rs == CPU->pending_write){
            CPU->true_dependency = YES;
        } else {  // Si no es el caso, indicamos que el registro rt queda pendiente por escribir
            CPU->pending_write = id_instr->rt;
        }
    }
    int true_dependency_after;
    if (CPU->true_dependency == NO) true_dependency_after = 0;
    else true_dependency_after = 1;

    // En caso de data hazard, añadir burbuja en la etapa EX1
    //if (CPU->true_dependency == YES && CPU->bubbles > 0) {
    //    add_to_pipeline(CPU, CPU->NOP, EX1);
    //    CPU->bubbles += 1;
    //}

    if (true_dependency_before == 0 && true_dependency_after == 1) return 1;
    return 0;

}

/**
 * Simula la etapa de operacion de la ALU principal (EX1).
 *
 * Para las instrucciones lw y sw, se calcula la direccion de memoria a la que apuntan para cargar o escribir,
 * respectivamente, y se copia esta direccion en el campo addr de la ram del CPU.
 *
 * Para el caso de las instrucciones beq y bne, en esta etapa se determina si su salto es tomado (BRANCH TAKEN) o no
 * (BRANCH NOT TAKEN).
 * @param CPU
 */
void ex1(CPU* CPU){
    Instruction* instr = CPU->executing[EX1];
    // Si la instruccion es LW, la ALU principal calcula la direccion del dato en memoria a cargar
    if (strcmp(instr->name, LW) == 0) {
        // $rs + Immediate para calcular la direccion del dato a cargar
        Register* rs = CPU->reg_file[instr->rs];
        int decimal_rs = binary_to_decimal(rs->data);
        int decimal_addr = decimal_rs + instr->immediate;
        if (CPU->ram_memory->addr != NULL) free(CPU->ram_memory->addr);
        CPU->ram_memory->addr = decimal_to_binary(decimal_addr, 32);
    // Si la instruccion es SW, la ALU principal calcula la direccion donde se escribira el dato en memoria
    } else if (strcmp(instr->name, SW) == 0){
        // $rs + Immediate para calcular la direccion del dato a escribir
        Register* rs = CPU->reg_file[instr->rs];
        int decimal_rs = binary_to_decimal(rs->data);
        int decimal_addr = decimal_rs + instr->immediate;
        if (CPU->ram_memory->addr != NULL) free(CPU->ram_memory->addr);
        CPU->ram_memory->addr = decimal_to_binary(decimal_addr, 32);
        // Indicamos que el dato a escribir es el dato almacenado en el rt de la instruccion
        Register* rt = CPU->reg_file[instr->rt];
        if (CPU->ram_memory->write_data != NULL) free(CPU->ram_memory->write_data);
        CPU->ram_memory->write_data = (char*)malloc(sizeof(char)*33);
        strcpy(CPU->ram_memory->write_data, rt->data);
    // Si la instruccion es BEQ, la ALU principal compara los campos rs y rt para habilitar la ALU que
    // calcula la direccion del jump
    } else if (strcmp(instr->name, BEQ) == 0){
        // Comparamos los campos rs con rt de la instruccion
        Register* rs = CPU->reg_file[instr->rs];
        Register* rt = CPU->reg_file[instr->rt];
        // Si son iguales, activamos la jump_alu (simula un multiplexor)
        if (strcmp(rs->data, rt->data) == 0){
            CPU->jump_alu_enabled = 1;
        } else {
            CPU->jump_alu_enabled = 0;
        }
    // Si la instruccion es BNE, la ALU principal compara los campos rs y rt para habilitar la ALU que
    // calcula la direccion del jump
    } else if (strcmp(instr->name, BNE) == 0){
        // Comparamos los campos rs con rt de la instruccion
        Register* rs = CPU->reg_file[instr->rs];
        Register* rt = CPU->reg_file[instr->rt];
        // Si no son iguales, activamos la jump_alu (simula un multiplexor)
        if (strcmp(rs->data, rt->data) != 0){
            CPU->jump_alu_enabled = 1;
        } else {
            CPU->jump_alu_enabled = 0;
        }
    } else if (strcmp(instr->name, J) == 0){
        CPU->jump_alu_enabled = 1;
    }
}

/**
 * Simula la operación de la ALU secundaria. Calcula las direcciones de memoria para los saltos de las instrucciones
 * j, beq y bne.
 * En el caso de una bifurcación tomada (BRANCH TAKEN), se realiza un vaciado de las instrucciones que están en
 * las etapas IF, ID y EX1 en el ciclo actual.
 *
 * @return 1 si se realiza un BRANCH TAKEN. 0 en caso contrario
 * @param CPU
 */
int ex2(CPU* CPU){
    if (CPU->jump_alu_enabled == 1){
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
                Instruction* j_instr = search_by_label(CPU->instr_memory, instr->j_address);
                // Actualizamos el valor del PC por la direccion de la instruccion con la etiqueta
                free(CPU->PC);
                CPU->PC = (char*)malloc(sizeof(char)*33);
                strcpy(CPU->PC, j_instr->address);
            }
            // Cuando existe un salto, es imposible un data hazard porque la instruccion que le precede está en MEM
            CPU->true_dependency = NO;
            CPU->pending_write = -1;
            // Como se hizo el branch, es necesario vaciar las instrucciones erroneamente leidas
            CPU->executing[IF] = CPU->NOP;
            CPU->executing[ID] = CPU->NOP;
            CPU->executing[EX1] = CPU->NOP;
            //CPU->bubbles = 0;
            return 1;
        // Instrucciones beq y bne
        } else if (strcmp(instr->name, BEQ) == 0 || strcmp(instr->name, BNE) == 0){
            // Si el immediate de la instruccion es un numero
            if (is_number(instr->j_address) == 1){
                // Lo multiplicamos por 4 y se lo sumamos al PC
                char* imm_32 = decimal_to_binary(binary_to_decimal(instr->j_address)*4, 32);
                char* current_PC = (char*)malloc(sizeof(char)*33);
                strcpy(current_PC, CPU->PC);
                free(CPU->PC);
                CPU->PC = two_binary_sum(imm_32, current_PC);
                free(imm_32);
                free(current_PC);
            } else {  // Si es una etiqueta
                // Buscamos la instruccion con dicha etiqueta en la memoria de instrucciones
                Instruction* j_instr = search_by_label(CPU->instr_memory, instr->j_address);
                // Actualizamos el valor del PC por la direccion de la instruccion con la etiqueta
                free(CPU->PC);
                CPU->PC = (char*)malloc(sizeof(char)*33);
                strcpy(CPU->PC, j_instr->address);
            }
            // Cuando existe un salto, es imposible un data hazard porque la instruccion que le precede está en MEM
            CPU->true_dependency = NO;
            CPU->pending_write = -1;
            // Como se hizo el branch, es necesario vaciar las instrucciones erroneamente leidas
            CPU->executing[IF] = CPU->NOP;
            CPU->executing[ID] = CPU->NOP;
            CPU->executing[EX1] = CPU->NOP;
            //CPU->bubbles = 0;
            return 1;
        }

    }
    return 0;
}

/**
 * Simula la etapa de lectura y escritura en memoria (MEM).
 *
 * Si la instrucción que está en MEM es lw, entonces se busca en la ram el dato que tiene la dirección indicada en el
 * campo addr de la ram. Este dato leído se copia y se añade a la cola read_data de la ram. En el caso de no existir
 * un dato con la dirección indicada, se carga el valor 0.
 *
 * Si la instruccion que está en MEM es sw, entonces se copia el dato que está en el campo write_data de la ram y se
 * añade a la memoria (capo data_list de la ram) con la dirección indicada en el campo addr de la ram. En el caso de
 * que ya exista un dato en la dirección indicada, este se reemplaza. En caso contrario, se crea una nueva entrada en
 * la ram.
 * @param CPU
 */
void mem(CPU* CPU){
    Instruction* instr = CPU->executing[MEM];
    if (strcmp(instr->name, LW) == 0){
        read_from_ram(CPU->ram_memory);
    } else if (strcmp(instr->name, SW) == 0){
        write_to_ram(CPU->ram_memory);
    }
}

/**
 * Simula la etapa de write back (WB).
 * Si en el ciclo que se llama, se satisface una dependencia (data hazard), retorna el numero de burbujas que fueron
 * necesarias. En caso contrario, retorna 0.
 * @param CPU
 * @return Cantidad de burbujas necesarias para satisfacer el data hazard.
 */
int write_back(CPU* CPU){
    // Write Back
    Instruction* wb_instr = CPU->executing[WB];
    Register* rs;
    Register* rt;
    Register* rd;
    int bubbles_needed = 0;
    if (strcmp(wb_instr->name, LW) == 0){
        rt = CPU->reg_file[wb_instr->rt];
        free(rt->data);
        rt->data = (char*)malloc(sizeof(char)*33);
        // Copiar el dato almacenado en read_data de la memoria ram, en el registro rt
        char* read_data = (char*)pop_from_list(CPU->ram_memory->read_data);
        strcpy(rt->data, read_data);
        free(read_data);
        // Si el registro rt estaba ocasionando una dependencia (data pending_write), indicar que ya se
        // satisfizo
        if (wb_instr->rt == CPU->pending_write){
            CPU->true_dependency = NO;
            CPU->pending_write = -1;
            // printf("Data Hazard solucionado con %d burbujas\n", CPU->bubbles);
            bubbles_needed = CPU->bubbles;
            CPU->bubbles = 0;
        }
    } else if (strcmp(wb_instr->name, SUB) == 0){
        rs = CPU->reg_file[wb_instr->rs];
        rt = CPU->reg_file[wb_instr->rt];
        rd = CPU->reg_file[wb_instr->rd];
        // $rd = $rs - $rt
        free(rd->data);
        rd->data = two_binary_rest(rs->data, rt->data);
        // Si el registro rd estaba ocasionando una dependencia (data pending_write), indicar que ya se
        // satisfizo
        if (wb_instr->rd == CPU->pending_write){
            CPU->true_dependency = NO;
            CPU->pending_write = -1;
            // printf("Data Hazard solucionado con %d burbujas\n", CPU->bubbles);
            bubbles_needed = CPU->bubbles;
            CPU->bubbles = 0;
        }
    } else if ((strcmp(wb_instr->name, ADD) == 0)){
        rs = CPU->reg_file[wb_instr->rs];
        rt = CPU->reg_file[wb_instr->rt];
        rd = CPU->reg_file[wb_instr->rd];
        // $rd = $rs + $rt
        free(rd->data);
        rd->data = two_binary_sum(rs->data, rt->data);
        // Si el registro rd estaba ocasionando una dependencia (data pending_write), indicar que ya se
        // satisfizo
        if (wb_instr->rd == CPU->pending_write){
            CPU->true_dependency = NO;
            CPU->pending_write = -1;
            // printf("Data Hazard solucionado con %d burbujas\n", CPU->bubbles);
            bubbles_needed = CPU->bubbles;
            CPU->bubbles = 0;
        }
    }else if ((strcmp(wb_instr->name, ADDI) == 0) || (strcmp(wb_instr->name, SUBI) == 0)){
        rs = CPU->reg_file[wb_instr->rs];
        rt = CPU->reg_file[wb_instr->rt];
        // $rt = $rs + Immediate
        int decimal_rs = binary_to_decimal(rs->data);
        free(rt->data);
        rt->data = decimal_to_binary(decimal_rs + wb_instr->immediate, 32);
        // Si el registro rt estaba ocasionando una dependencia (data pending_write), indicar que ya se
        // satisfizo
        if (wb_instr->rt == CPU->pending_write){
            CPU->true_dependency = NO;
            CPU->pending_write = -1;
            // printf("Data Hazard solucionado con %d burbujas\n", CPU->bubbles);
            bubbles_needed = CPU->bubbles;
            CPU->bubbles = 0;
        }
    }
    return bubbles_needed;
}


/**
 * Identifica si hay 6 NOPs en el pipeline (en ejecucion)
 * @param cpu
 * @return 1 si hay 6 NOPs. 0 en caso contrario
 */
int are_there_six_nops(CPU* cpu){
    if (strcmp(cpu->executing[IF]->name, NOP) == 0 &&
        strcmp(cpu->executing[ID]->name, NOP) == 0 &&
        strcmp(cpu->executing[EX1]->name, NOP) == 0 &&
        strcmp(cpu->executing[EX2]->name, NOP) == 0 &&
        strcmp(cpu->executing[MEM]->name, NOP) == 0 &&
        strcmp(cpu->executing[WB]->name, NOP) == 0){
        return 1;
    }
    return 0;
}

/**
 * Ejecuta las instrucciones almacenadas
 */
void run(CPU* CPU){
    CPU->cc = 0;
    //print_pipeline(CPU);
    //printf("PRESIONE [ENTER] PARA CONTINUAR ...");
    //while (getchar() != '\n');
    CPU->cc = 1;
    instruction_fetch(CPU);
    write_back(CPU);
    instruction_decode(CPU);
    ex1(CPU);
    ex2(CPU);
    mem(CPU);
    print_pipeline(CPU, 24);
    if (strcmp(CPU->executing[IF]->name, BEQ) == 0 || strcmp(CPU->executing[IF]->name, BNE) == 0){
        printf(" WARNING: Control hazard detectado. Linea:%d, CC:%d\n", CPU->executing[IF]->line, CPU->cc);
    } else {
        printf("\n");
    }
    printf("+%.*s", 7, "---------");
    printf("+%.*s", 24, "-------------------------------------------------");
    printf("+%.*s", 24, "-------------------------------------------------");
    printf("+%.*s", 24, "-------------------------------------------------");
    printf("+%.*s", 24, "-------------------------------------------------");
    printf("+%.*s", 24, "-------------------------------------------------");
    printf("+%.*s+\n", 24, "-------------------------------------------------");
    //printf("PRESIONE [ENTER] PARA CONTINUAR ...");
    //while (getchar() != '\n');
    while (are_there_six_nops(CPU) == 0){
        CPU->cc += 1;
        int bubbles_needed = write_back(CPU);
        instruction_fetch(CPU);
        int data_hazard = instruction_decode(CPU);
        if (data_hazard == 1){
            print_pipeline(CPU, 24);
            printf(" WARNING: Data hazard detectado. Linea:%d, CC:%d\n", CPU->executing[ID]->line, CPU->cc);
            printf("+%.*s", 7, "----------");
            printf("+%.*s", 24, "-------------------------------------------------");
            printf("+%.*s", 24, "-------------------------------------------------");
            printf("+%.*s", 24, "-------------------------------------------------");
            printf("+%.*s", 24, "-------------------------------------------------");
            printf("+%.*s", 24, "-------------------------------------------------");
            printf("+%.*s+\n", 24, "-------------------------------------------------");
            // add_to_pipeline(CPU, CPU->NOP, EX1);
            // CPU->bubbles += 1;
            // CPU->cc += 1;
        }
        ex1(CPU);

        int branch_taken = ex2(CPU);
        mem(CPU);
        if (CPU->true_dependency == YES) {
            add_to_pipeline(CPU, CPU->NOP, EX1);
            CPU->bubbles += 1;
            CPU->cc += 1;
        }
        print_pipeline(CPU, 24);

        int new_line_printed = 0;
        if (strcmp(CPU->executing[IF]->name, BEQ) == 0 || strcmp(CPU->executing[IF]->name, BNE) == 0){
            printf(" WARNING: Control hazard detectado. Linea:%d, CC:%d\n", CPU->executing[IF]->line, CPU->cc);
            new_line_printed = 1;
        }

        if (bubbles_needed != 0){
            printf(" Data hazard solucionado con %d burbujas\n", bubbles_needed);
            new_line_printed = 1;
        }

        if (branch_taken == 1){
            if (CPU->bubbles > 0){
                printf(" Data hazard solucionado con %d burbuja(s). Se realizó un salto", CPU->bubbles);
                CPU->bubbles = 0;
            }
            if (strcmp(CPU->executing[EX2]->name, J) != 0) {
                printf(" BRANCH TAKEN, se agregaron 3 burbujas para solucionar el Control hazard\n");
                new_line_printed = 1;
            }
        }
        if (branch_taken == 0){
            if (strcmp(CPU->executing[EX2]->name, BEQ) == 0 || strcmp(CPU->executing[EX2]->name, BNE) == 0){
                printf(" No fue necesario agregar burbujas. Se cumplio la prediccion BRANCH NOT TAKEN\n");
                new_line_printed = 1;
            }
        }
        if (new_line_printed == 0) printf("\n");

        printf("+%.*s", 7, "---------");
        printf("+%.*s", 24, "-------------------------------------------------");
        printf("+%.*s", 24, "-------------------------------------------------");
        printf("+%.*s", 24, "-------------------------------------------------");
        printf("+%.*s", 24, "-------------------------------------------------");
        printf("+%.*s", 24, "-------------------------------------------------");
        printf("+%.*s+\n", 24, "-------------------------------------------------");



        //printf("PRESIONE [ENTER] PARA CONTINUAR ...");
        //while (getchar() != '\n');
    }
}

/*
 * Imprime las instrucciones en ejecucion (Pipeline)
 */
void print_pipeline(CPU* cpu, int cell_size){
    char cc_string[12];
    sprintf(cc_string, "%d", cpu->cc);
    char* if_instr = instr_to_string(cpu->executing[IF], cpu->reg_file);
    char* id_instr = instr_to_string(cpu->executing[ID], cpu->reg_file);
    char* ex1_instr = instr_to_string(cpu->executing[EX1], cpu->reg_file);
    char* ex2_instr = instr_to_string(cpu->executing[EX2], cpu->reg_file);
    char* mem_instr = instr_to_string(cpu->executing[MEM], cpu->reg_file);
    char* wb_instr = instr_to_string(cpu->executing[WB], cpu->reg_file);
    int if_instr_size = strlen(if_instr);
    int id_instr_size = strlen(id_instr);
    int ex1_instr_size = strlen(ex1_instr);
    int ex2_instr_size = strlen(ex2_instr);
    int mem_instr_size = strlen(mem_instr);
    int wb_instr_size = strlen(wb_instr);
    int cc_string_size = strlen(cc_string);
    int i = 0;
    printf("|%d", cpu->cc);
    // Imprimir tantos ' ' como sea posible para llegar al largo de la celda CC
    while (i < 9-cc_string_size-2) {
        printf(" ");
        i++;
    }
    i = 0;
    printf("|");
    printf("%s", if_instr);
    while (i < cell_size-if_instr_size){
        printf("%s", " ");
        i++;
    }
    i = 0;
    printf("|");
    printf("%s", id_instr);
    while (i < cell_size-id_instr_size){
        printf("%s", " ");
        i++;
    }
    i = 0;
    printf("|");
    printf("%s", ex1_instr);
    while (i < cell_size-ex1_instr_size){
        printf("%s", " ");
        i++;
    }
    i = 0;
    printf("|");
    printf("%s", ex2_instr);
    while (i < cell_size-ex2_instr_size){
        printf("%s", " ");
        i++;
    }
    i = 0;
    printf("|");
    printf("%s", mem_instr);
    while (i < cell_size-mem_instr_size){
        printf("%s", " ");
        i++;
    }
    i = 0;
    printf("|");
    printf("%s", wb_instr);
    while (i < cell_size-wb_instr_size){
        printf("%s", " ");
        i++;
    }
    printf("|");
}

/*
 * Imprime la memoria de instrucciones.
 */
void print_instr_mem(CPU* cpu){
    Node* current_instr = cpu->instr_memory->first;
    while (current_instr != NULL){
        print_instr((Instruction*)current_instr->data,cpu->reg_file);
        printf("\n");
        current_instr = current_instr->next;
    }
}
/*
 * Libera la memoria utilizada por el procesador
 */
void free_cpu(CPU* cpu){
    free(cpu->PC);
    free_reg_file(cpu->reg_file);
    free(cpu->executing);
    free_ram_mem(cpu->ram_memory);
    free_list_and_nodes(cpu->instr_memory, free_instr, 0);
    free_instr(cpu->NOP);
    free(cpu);
}