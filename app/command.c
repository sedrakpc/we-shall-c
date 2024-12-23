#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <dirent.h>
#include "../include/debug.h"
#include "../include/command.h"
#include "../include/array_list.h"

// built-in commands
char COMMANDS[3][10] = {
        "exit",
        "echo",
        "type"
};
ArrayList *PATH_COMMANDS;
int NUMBER_OF_SUPPORTED_COMMANDS = sizeof(COMMANDS) / sizeof(COMMANDS[0]);
char* PATH_DELIMITERS = ":";
char* PATH_ENV = "/usr/bin";

void setPath(char* path) {
    PATH_ENV = path;
}

int isValidCommand(char* command) {
    for (int i = 0; i < NUMBER_OF_SUPPORTED_COMMANDS; i++) {
        if (strcmp(command, COMMANDS[i]) == 0) {
            return 1;
        }
    }
    for(int i = 0; i < PATH_COMMANDS->size; i++) {
//        data contains command and path separated by ':'
        char *path_cmd = strtok(PATH_COMMANDS->data[i], PATH_DELIMITERS);
        if (strcasecmp(command, path_cmd) == 0) {
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
    if (strcasecmp(command, "type") == 0) {
        if (argsCount != 2) {
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
    } else if(strcasecmp(command, "type") == 0) {
        if (validateArguments(command, args, argsCount)) {
            for(int i = 0; i < NUMBER_OF_SUPPORTED_COMMANDS; i++) {
                if (strcasecmp(args[1], COMMANDS[i]) == 0) {
                    printf("%s is a shell builtin\n", args[1]);
                    return 1;
                }
            }
            for(int i = 0; i < PATH_COMMANDS->size; i++) {
                char *temp = malloc(strlen(PATH_COMMANDS->data[i]) + 1);
                strcpy(temp, PATH_COMMANDS->data[i]);
                char *path_cmd = strtok(temp, PATH_DELIMITERS);
                char *path = strtok(NULL, PATH_DELIMITERS);
                if (strcasecmp(args[1], path_cmd) == 0) {
                    printf("%s is %s\n", path_cmd, path);
                    return 1;
                }
            }
            printf("%s: not found\n", args[1]);
        }
    }
    return 0;
}

int reloadPathCommands() {
//    printf("PATH: %s\n", PATH_ENV);
    PATH_COMMANDS = createArrayList(1000);
    DIR *d;
    struct dirent *dir;
    char* path = strdup(PATH_ENV);
    char *token = strtok(path, PATH_DELIMITERS);
//    printf("Token: %s\n", token);
    while (token != NULL) {
//        printf("Token: %s\n", token);
        d = opendir(token);
        if (d) {
            while ((dir = readdir(d)) != NULL) {
                if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
                    continue;
                }
                char pathCommand[255] = "";
                strcat(pathCommand, dir->d_name);
                strcat(pathCommand, PATH_DELIMITERS);
                strcat(pathCommand, token);
                strcat(pathCommand, "/");
                strcat(pathCommand, dir->d_name);
                TRACE(("%s\n", pathCommand));
                append(PATH_COMMANDS, pathCommand);
            }
            closedir(d);
        }
        token = strtok(NULL, PATH_DELIMITERS);
    }
    return 0;
}