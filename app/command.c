#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "../include/command.h"

char COMMANDS[2][10] = {
        "exit",
        "echo"
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

int validateArguments(char* command, char* args[], int argsCount) {
    if (strcasecmp(command, "exit") == 0) {
        if (argsCount < 2) {
            printf("invalid number of arguments\n");
            return 0;
        }
    }
    if (strcasecmp(command, "echo") == 0) {
        if (argsCount < 2) {
            printf("invalid number of arguments\n");
            return 0;
        }
    }
    return 1;
}

int processCommand(char* command, char* args[], int argsCount) {
    if (strcasecmp(command, "exit") == 0) {
        if (validateArguments(command, args, argsCount)) {
            exit(atoi(args[1]));
        }
    } else if (strcasecmp(command, "echo") == 0) {
        if (validateArguments(command, args, argsCount)) {
            for (int i = 1; i < argsCount; i++) {
                printf("%s ", args[i]);
            }
            printf("\n");
            return 1;
        }
    }
    return 0;
}