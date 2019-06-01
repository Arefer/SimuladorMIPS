#include "../Auxiliar/binary_ops.h"
#include "../Auxiliar/custom_string_ops.h"
#include "register.h"
#include "instr_mem.h"
#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*
 * Lee el archivo con el codigo fuente
 * Retorna la memoria de instrucciones (Instr_memory)
 */
void read_source_code(char* filePath, Cpu* cpu){
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
        // Tipo R
        if (strcmp(line, "add") == 0 ||
            strcmp(line, "sub") == 0 ||
            strcmp(line, "and") == 0 ||
            strcmp(line, "or") == 0  ||
            strcmp(line, "slt") == 0){
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
            Register* reg;
            // Guardar el tipo
            instr->type = 'R';
            // Guardar el nombre
            instr->name = (char*)malloc(sizeof(char)*(strlen(line)+1));
            strcpy(instr->name, line);

            // $rd
            instr->rd = (char*)malloc(sizeof(char)*33);
            fscanf(f, "%s", aux);
            // Quitarle la coma final y guardarlo
            memcpy(instr->rd, aux, strlen(aux)-1);
            reg = searchRegister(cpu->reg_file, instr->rd);
            strcpy(instr->rd, reg->number);

            // $rs
            instr->rs = (char*)malloc(sizeof(char)*33);
            fscanf(f, "%s", aux);
            // Quitarle la coma final y guardarlo
            memcpy(instr->rs, aux, strlen(aux)-1);
            // Buscamos el registro por su nombre
            reg = searchRegister(cpu->reg_file, instr->rs);
            strcpy(instr->rs, reg->number);

            // $rt
            instr->rt = (char*)malloc(sizeof(char)*33);
            fscanf(f, "%s", aux);
            // Quitarle el salto de linea al final y guardarlo
            memcpy(instr->rt, aux, strlen(aux));
            reg = searchRegister(cpu->reg_file, instr->rt);
            strcpy(instr->rt, reg->number);

            // Leer lo que resta de linea (puede haber espacios)
            fscanf(f, "%[^\n]s", aux);

            // Leer el salto de linea
            fgetc(f);

            free(aux);
            add_to_list(cpu->instr_memory, instr, instr->address);
        }

        // Instruccion lw y sw
        if (strcmp(line, "lw") == 0 || strcmp(line, "sw") == 0){
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
            Register* reg;
            // Guardar el tipo
            instr->type = 'I';
            // Guardar el nombre
            instr->name = (char*)malloc(sizeof(char)*(strlen(line)+1));
            strcpy(instr->name, line);

            // $rt
            instr->rt = (char*)malloc(sizeof(char)*33);
            fscanf(f, "%s", aux);
            // Quitar la coma
            memcpy(instr->rt, aux, strlen(aux)-1);
            reg = searchRegister(cpu->reg_file, instr->rt);
            strcpy(instr->rt, reg->number);
            
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
            instr->rs = (char*)malloc(sizeof(char)*33);
            // Leer hasta encontrar un parentesis
            fscanf(f, "%[^)]s", aux);
            reg = searchRegister(cpu->reg_file, aux);
            strcpy(instr->rs, reg->number);

            // Leer lo que resta de linea (puede haber espacios)
            fscanf(f, "%[^\n]s", aux);
            // Leer el salto de linea
            fgetc(f);

            free(aux);
            add_to_list(cpu->instr_memory, instr, instr->address);
        }

        // Instruccion addi
        if (strcmp(line, "addi") == 0){
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
            Register* reg;
            // Guardar el tipo
            instr->type = 'I';
            // Guardar el nombre
            instr->name = (char*)malloc(sizeof(char)*(strlen(line)+1));
            strcpy(instr->name, line);

            // $rt
            instr->rt = (char*)malloc(sizeof(char)*33);
            fscanf(f, "%s", aux);
            // Quitar la coma
            memcpy(instr->rt, aux, strlen(aux)-1);
            reg = searchRegister(cpu->reg_file, instr->rt);
            strcpy(instr->rt, reg->number);

            // $rs
            instr->rs = (char*)malloc(sizeof(char)*33);
            fscanf(f, "%s", aux);
            // Quitar la coma
            memcpy(instr->rs, aux, strlen(aux)-1);
            reg = searchRegister(cpu->reg_file, instr->rs);
            strcpy(instr->rs, reg->number);

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

        // Instruccion beq
        if (strcmp(line, "beq") == 0){
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
            Register* reg;
            // Guardar el tipo
            instr->type = 'I';
            // Guardar el nombre
            instr->name = (char*)malloc(sizeof(char)*(strlen(line)+1));
            strcpy(instr->name, line);

            // $rs
            instr->rs = (char*)malloc(sizeof(char)*33);
            fscanf(f, "%s", aux);
            // Quitar la coma
            memcpy(instr->rs, aux, strlen(aux)-1);
            reg = searchRegister(cpu->reg_file, instr->rs);
            strcpy(instr->rs, reg->number);

            // $rt
            instr->rt = (char*)malloc(sizeof(char)*33);
            fscanf(f, "%s", aux);
            // Quitar la coma
            memcpy(instr->rt, aux, strlen(aux)-1);
            reg = searchRegister(cpu->reg_file, instr->rt);
            strcpy(instr->rt, reg->number);

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
        if (strcmp(line, "j") == 0){
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
            instr->name = (char*)malloc(sizeof(char)*(strlen(line)+1));
            strcpy(instr->name, line);

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

