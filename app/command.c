#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "../include/command.h"

char COMMANDS[1][10] = {
        "exit"
};
int NUMBER_OF_SUPPORTED_COMMANDS = sizeof(COMMANDS) / sizeof(COMMANDS[0]);


int isValidCommand(char* command) {
    for (int i = 0; i < NUMBER_OF_SUPPORTED_COMMANDS; i++) {
        if (strcmp(command, COMMANDS[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int processCommand(char* command, char* args[], int argsCount) {
    if (strcasecmp(command, "exit") == 0) {
        if (argsCount < 2) {
            printf("invalid number of arguments\n");
        } else {
            exit(atoi(args[1]));
        }
        return 1;
    }
    return 0;
}