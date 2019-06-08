#include "../Auxiliar/binary_ops.h"
#include "../Auxiliar/custom_string_ops.h"
#include "register.h"
#include "instr_mem.h"
#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <elf.h>


/*
 * Lee el archivo con el codigo fuente
 * Los nombres de las instrucciones estan almacenados en constantes (ver archivo instr_mem.h)
 */
void read_source_code(char* filePath, CPU* cpu){
    // Leer el archivo
    FILE* f = fopen(filePath, "r");
    if (f == NULL){
        printf("Error al abrir el codigo fuente\n");
        exit(-1);
    }

    const size_t line_size = 300;
    char* line = (char*)malloc(sizeof(char)*line_size);
    char* label = (char*)malloc(sizeof(char)*100);
    int hasLabel = 0;
    int line_number = 1;
    
    while (fscanf(f, "%s", line) == 1)  {
        // Comprobar si la linea es una etiqueta
        if (line[strlen(line)-1] == ':'){
            // Copiar la etiqueta sin los ":"
            memcpy(label, line, strlen(line)-1);
            label[strlen(line)-1] = '\0';
            hasLabel = 1;

            // Leer lo que resta de linea (puede haber espacios)
            char* aux = (char*)malloc(sizeof(char)*200);
            fscanf(f, "%[^\n]s", aux);
            free(aux);

            // Leer el salto de linea
            fgetc(f);
        }
        // Instrucciones add y sub
        if (strcmp(line, ADD) == 0 || strcmp(line, SUB) == 0){
            Instruction* instr = init_instr();
            instr->line = line_number;
            if (hasLabel == 1){
                instr->label = (char*)malloc(sizeof(char)*(strlen(label)+1));
                strcpy(instr->label, label);
                hasLabel = 0;
            }
            if (cpu->instr_memory->length == 0)
                instr->address = decimal_to_binary(0, 32);
            else{
                Instruction* last = (Instruction*) cpu->instr_memory->last->data;
                instr->address = binary_sum(last->address, 4, 32);
            }

            char* aux = (char*)malloc(sizeof(char)*300);
            // Guardar el tipo
            instr->type = 'R';
            // Guardar el nombre
            if (strcmp(line, ADD) == 0)
                instr->name = ADD;
            else
                instr->name = SUB;
            // $rd
            char* rd = (char*)malloc(sizeof(char)*33);
            fscanf(f, "%s", aux);
            // Quitarle la coma final y guardarlo
            memcpy(rd, aux, strlen(aux)-1);
            instr->rd = search_register(cpu->reg_file, rd);
            free(rd);

            // $rs
            char* rs = (char*)malloc(sizeof(char)*33);
            fscanf(f, "%s", aux);
            // Quitarle la coma final y guardarlo
            memcpy(rs, aux, strlen(aux)-1);
            // Buscamos el registro por su nombre
            instr->rs = search_register(cpu->reg_file, rs);
            free(rs);

            // $rt
            fscanf(f, "%s", aux);
            // Quitarle el salto de linea al final y guardarlo
            instr->rt = search_register(cpu->reg_file, aux);

            // Leer lo que resta de linea (puede haber espacios)
            fscanf(f, "%[^\n]s", aux);

            // Leer el salto de linea
            fgetc(f);

            free(aux);
            add_to_list(cpu->instr_memory, instr, instr->address);
        }

        // Instrucciones lw y sw
        if (strcmp(line, LW) == 0 || strcmp(line, SW) == 0){
            Instruction* instr = init_instr();
            instr->line = line_number;
            if (hasLabel == 1){
                instr->label = (char*)malloc(sizeof(char)*(strlen(label)+1));
                strcpy(instr->label, label);
                hasLabel = 0;
            }
            if (cpu->instr_memory->length == 0)
                instr->address = decimal_to_binary(0, 32);
            else{
                Instruction* last = (Instruction*) cpu->instr_memory->last->data;
                instr->address = binary_sum(last->address, 4, 32);
            }
            char* aux = (char*)malloc(sizeof(char)*300);
            // Guardar el tipo
            instr->type = 'I';
            // Guardar el nombre
            if (strcmp(line, LW) == 0)
                instr->name = LW;
            else
                instr->name = SW;

            // $rt
            char* rt = (char*)malloc(sizeof(char)*33);
            fscanf(f, "%s", aux);
            // Quitar la coma
            memcpy(rt, aux, strlen(aux)-1);
            instr->rt = search_register(cpu->reg_file, rt);
            free(rt);
            
            // Immediate
            // Leer hasta encontrar un parentesis
            fscanf(f, "%[^(]s", aux);
            // Quitarle los espacios
            int auxint = 0;
            char** splitted = split(aux, " ", &auxint);
            instr->immediate =  atoi(splitted[0]);
            free_splitted(splitted, auxint);
            // Leer el "("
            fgetc(f);

            // $rs
            // Leer hasta encontrar un parentesis
            fscanf(f, "%[^)]s", aux);
            instr->rs = search_register(cpu->reg_file, aux);


            // Leer lo que resta de linea (puede haber espacios)
            fscanf(f, "%[^\n]s", aux);
            // Leer el salto de linea
            fgetc(f);

            free(aux);
            add_to_list(cpu->instr_memory, instr, instr->address);
        }

        // Instrucciones addi y subi
        if (strcmp(line, ADDI) == 0 || strcmp(line, SUBI) == 0){
            Instruction* instr = init_instr();
            instr->line = line_number;
            if (hasLabel == 1){
                instr->label = (char*)malloc(sizeof(char)*(strlen(label)+1));
                strcpy(instr->label, label);
                hasLabel = 0;
            }
            if (cpu->instr_memory->length == 0)
                instr->address = decimal_to_binary(0, 32);
            else{
                Instruction* last = (Instruction*) cpu->instr_memory->last->data;
                instr->address = binary_sum(last->address, 4, 32);
            }
            char* aux = (char*)malloc(sizeof(char)*300);
            // Guardar el tipo
            instr->type = 'I';
            // Guardar el nombre
            if (strcmp(line, ADDI) == 0)
                instr->name = ADDI;
            else
                instr->name = SUBI;

            // $rt
            char* rt = (char*)malloc(sizeof(char)*33);
            fscanf(f, "%s", aux);
            // Quitar la coma
            memcpy(rt, aux, strlen(aux)-1);
            instr->rt = search_register(cpu->reg_file, rt);
            free(rt);

            // $rs
            char* rs = (char*)malloc(sizeof(char)*33);
            fscanf(f, "%s", aux);
            // Quitar la coma
            memcpy(rs, aux, strlen(aux)-1);
            instr->rs = search_register(cpu->reg_file, rs);
            free(rs);

            // Immediate
            fscanf(f, "%s", aux);
            instr->immediate = atoi(aux);

            // Leer lo que resta de linea (puede haber espacios)
            fscanf(f, "%[^\n]s", aux);

            // Leer el salto de linea
            fgetc(f);
            free(aux);
            add_to_list(cpu->instr_memory, instr, instr->address);
        }

        // Instrucciones beq y bne
        if (strcmp(line, BEQ) == 0 || strcmp(line, BNE) == 0){
            Instruction* instr = init_instr();
            instr->line = line_number;
            if (hasLabel == 1){
                instr->label = (char*)malloc(sizeof(char)*(strlen(label)+1));
                strcpy(instr->label, label);
                hasLabel = 0;
            }
            if (cpu->instr_memory->length == 0)
                instr->address = decimal_to_binary(0, 32);
            else{
                Instruction* last = (Instruction*) cpu->instr_memory->last->data;
                instr->address = binary_sum(last->address, 4, 32);
            }
            char* aux = (char*)malloc(sizeof(char)*300);
            // Guardar el tipo
            instr->type = 'I';
            // Guardar el nombre
            if (strcmp(line, BEQ) == 0)
                instr->name = BEQ;
            else
                instr->name = BNE;

            // $rs
            char* rs = (char*)malloc(sizeof(char)*33);
            fscanf(f, "%s", aux);
            // Quitar la coma
            memcpy(rs, aux, strlen(aux)-1);
            instr->rs = search_register(cpu->reg_file, rs);
            free(rs);

            // $rt
            char* rt = (char*)malloc(sizeof(char)*33);
            fscanf(f, "%s", aux);
            // Quitar la coma
            memcpy(rt, aux, strlen(aux)-1);
            instr->rt = search_register(cpu->reg_file, rt);
            free(rt);

            // Immediate
            fscanf(f, "%s", aux);
            instr->j_address = (char*)malloc(sizeof(char)*(strlen(aux)+1));
            strcpy(instr->j_address, aux);

            // Leer lo que resta de linea (puede haber espacios)
            fscanf(f, "%[^\n]s", aux);

            // Leer el salto de linea
            fgetc(f);
            free(aux);
            add_to_list(cpu->instr_memory, instr, instr->address);
        }

        // Instruccion j
        if (strcmp(line, J) == 0){
            Instruction* instr = init_instr();
            instr->line = line_number;
            if (hasLabel == 1){
                instr->label = (char*)malloc(sizeof(char)*(strlen(label)+1));
                strcpy(instr->label, label);
                hasLabel = 0;
            }
            if (cpu->instr_memory->length == 0)
                instr->address = decimal_to_binary(0, 32);
            else{
                Instruction* last = (Instruction*) cpu->instr_memory->last->data;
                instr->address = binary_sum(last->address, 4, 32);
            }
            char* aux = (char*)malloc(sizeof(char)*100);
            // Guardar el tipo
            instr->type = 'J';

            // Guardar el nombre
            instr->name = J;

            // address
            fscanf(f, "%s", aux);
            instr->j_address = (char*)malloc(sizeof(char)*(strlen(aux)+1));
            strcpy(instr->j_address, aux);

            // Leer lo que resta de linea (puede haber espacios)
            fscanf(f, "%[^\n]s", aux);

            // Leer el salto de linea
            fgetc(f);

            free(aux);
            add_to_list(cpu->instr_memory, instr, instr->address);
        }
        if (fgetc(f) == EOF) break;
        fseek( f, -1, SEEK_CUR );
        if (fgetc(f) == '\n') {
            line_number += 1;
        }
        // Retrocedemos el puntero para anular el adelantamiento provocado por el if anterior
        fseek( f, -1, SEEK_CUR );
        line_number += 1;
    }
    free(line);
    free(label);
    fclose(f);
}

/**
 * Escribe los valores de los registros en el momento en que es llamada.
 * @param path ruta del archivo a esribir
 * @param cpu procesador que contiene los registros
 */
void write_registers(char* path, CPU* cpu){
    FILE* f = fopen(path, "wr");
    if (f == NULL){
        printf("Error al escribir el archivo\n");
        exit(-1);
    }
    // Iterar los registros y escribirlos
    int i = 0;
    while (i < 32){
        Register* r = cpu->reg_file[i];
        int valor = binary_to_decimal(r->data);
        fprintf(f, "%s=%d\n", r->name, valor);
        i++;
    }
    printf("Archivo [%s] escrito correctamente\n", path);
    fclose(f);
}