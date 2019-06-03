//
// Created by kevin on 18-05-19.
//

#ifndef LAB3_CUSTOM_STRING_OPS_H
#define LAB3_CUSTOM_STRING_OPS_H
int is_number(char* string);
int piecesCount(char* s, char* c);
char** split(char* s, char* c, int* length);
void free_splitted(char** splitted, int large);
void free_string(void* s);
#endif //LAB3_CUSTOM_STRING_OPS_H
