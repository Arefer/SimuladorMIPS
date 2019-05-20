//
// Created by kevin on 18-05-19.
//

#include "binary_ops.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/**
 * Transforma un entero a binario (string) con 'bits' numero de bits
 */
char *decimal_to_binary(int n, int bits) {
    int c, d, count;
    char *pointer;

    count = 0;
    pointer = (char*)malloc(bits+1);

    if (pointer == NULL)
        exit(EXIT_FAILURE);

    for (c = bits-1 ; c >= 0 ; c--)
    {
        d = n >> c;

        if (d & 1)
            *(pointer+count) = 1 + '0';
        else
            *(pointer+count) = 0 + '0';

        count++;
    }
    *(pointer+count) = '\0';

    return  pointer;
}

/**
 * Transforma un binario a decimal
 */
int binary_to_decimal(char *bin) {
    int decimal = 0;
    int length = strlen(bin);
    int i = length - 1;
    while (i >= 0){
        if (bin[i] == '1'){
            decimal += (int) pow(2, length-(i+1));
        }
        i--;
    }
    return decimal;
}

/**
 * Suma un entero en binario
 */
char *binary_sum(char *bin, int n, int bits) {
    int dec = binary_to_decimal(bin);
    int result = dec + n;
    return decimal_to_binary(result, bits);
}


/**
 * Suma dos numeros binarios de la misma cantidad de bits
 */
char *two_binary_sum(char *bin1, char *bin2) {
    int dec1 = binary_to_decimal(bin1);
    int dec2 = binary_to_decimal(bin2);
    int res_dec = dec1+dec2;
    return decimal_to_binary(res_dec, strlen(bin1));
}


/**
 * Resta dos numeros binarios de la misma cantidad de bits
 */
char *two_binary_rest(char *bin1, char *bin2) {
    int dec1 = binary_to_decimal(bin1);
    int dec2 = binary_to_decimal(bin2);
    int res_dec = dec1-dec2;
    return decimal_to_binary(res_dec, strlen(bin1));
}


/**
 * Extiende el signo de un binario
 */
char *sign_extend(char *bin, int n) {
    int dec = binary_to_decimal(bin);
    return decimal_to_binary(dec, n);
}


/**
 * Logical OR -> retorna 1 si se cumple bin1 | bin2, 0 en caso contrario
 */
int logical_or(char *bin1, char *bin2) {
    int i = 0;
    while (i < 32){
        if (bin1[i] == '0' && bin2[i] == '0'){
            return 0;
        }
        i++;
    }
    return 1;
}
