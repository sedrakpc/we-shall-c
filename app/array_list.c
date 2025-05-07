#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/debug.h"
#include "../include/array_list.h"

ArrayList *createArrayList(int capacity) {
    ArrayList *list = (ArrayList *) malloc(sizeof(ArrayList));
    list->data = malloc(capacity * sizeof(char *));
    list->size = 0;
    list->capacity = capacity;
    return list;
}

void append(ArrayList *list, char *element) {
    if (list->size == list->capacity) {
        list->capacity *= 2;
        TRACE(("Reallocating memory to %d\n", list->capacity));
        list->data = realloc(list->data, list->capacity * sizeof(char *));
    }
    list->data[list->size] = malloc(strlen(element) + 1);
    strcpy(list->data[list->size], element);
    ++list->size;
}

void print(ArrayList *list) {
    printf("[");
    for (int i = 0; i < list->size; i++) {
        printf("%s", list->data[i]);
        if (i < list->size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

void freeArrayList(ArrayList *list) {
    free(list->data);
    free(list);
}