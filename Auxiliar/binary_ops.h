//
// Created by kevin on 18-05-19.
//

#ifndef LAB3_BINARY_OPS_H
#define LAB3_BINARY_OPS_H
char* decimal_to_binary(int n, int bits);
int binary_to_decimal(char* bin);
char* binary_sum(char* bin, int n, int bits);
char* two_binary_sum(char* bin1, char* bin2);
char* two_binary_rest(char* bin1, char* bin2);
char* sign_extend(char* bin, int n);
int logical_or(char* bin1, char* bin2);
#endif //LAB3_BINARY_OPS_H
