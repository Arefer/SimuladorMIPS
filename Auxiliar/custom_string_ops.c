//
// Created by kevin on 18-05-19.
//

#include "custom_string_ops.h"
#include <string.h>
#include <stdlib.h>

/**
 * Verifica si un string esta compuesto de solo numeros
 * Retorna 1 en caso de exito, 0 en caso contrario
 */
int is_number(char *string) {
    int i = 0;
    int ans = 1;
    while (i < strlen(string) && ans == 1){
        if(string[i] >= '0' && string[i] <= '9')
            i++;
        else
            ans = 0;
    }
    return ans;
}


/**
 * Determina la cantidad de "piezas" que tiene una string.
 * c = caracter separador
 * s = string
 */
int piecesCount(char *s, char *c) {
    // Copia del string original para no alterarlo
    char* copy = (char*)malloc(sizeof(char)*(strlen(s)+1));
    strcpy(copy, s);

    int pieces = 0;
    char* piece = strtok(copy, c);
    while (piece != NULL){
        pieces += 1;
        piece = strtok(NULL, c);
    }
    free(copy);
    return pieces;
}


/**
 * Separa un string (sin alterarlo) segun el caracter c.
 * s = string
 * c = char
 * Retorna el largo del char** obtenido en "length"
 */
char **split(char *s, char *c, int *length) {
    int pieces = piecesCount(s, c);
    *length = pieces;
    // Copia del string original para no alterarlo
    char* copy = (char*)malloc(sizeof(char)*(strlen(s)+1));
    strcpy(copy, s);

    if (pieces == 0){
        free(copy);
        return NULL;
    }

    char** splitted = (char**)malloc(sizeof(char*)*pieces);
    int i = 0;
    while (i < pieces){
        char* piece;
        if (i == 0){
            piece = strtok(copy, c);
        } else {
            piece = strtok(NULL, c);
        }
        splitted[i] = (char*)malloc(sizeof(char)*(strlen(piece)+1));
        strcpy(splitted[i], piece);
        i++;
    }
    free(copy);
    return splitted;
}


/**
 * Libera la memoria de un arreglo generado por split
 *
*/
void free_splitted(char** splitted, int large){
    int i = 0;
    while (i < large){
        free(splitted[i]);
        i++;
    }
    free(splitted);
}

/*
 * Wrapper de free que recibe un void* y lo castea a char* para liberar su memoria
 */
void free_string(void* s){
    char* string = (char*)s;
    free(string);
}