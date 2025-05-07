//
// Created by Sedrak Dalaloyan on 12/21/24.
//

#ifndef CODECRAFTERS_SHELL_C_ARRAY_LIST_H
#define CODECRAFTERS_SHELL_C_ARRAY_LIST_H


typedef struct {
    int size;
    int capacity;
    char **data;
} ArrayList;

ArrayList *createArrayList(int capacity);

void append(ArrayList *list, char *element);

void print(ArrayList *list);

void freeArrayList(ArrayList *list);

#endif //CODECRAFTERS_SHELL_C_ARRAY_LIST_H
