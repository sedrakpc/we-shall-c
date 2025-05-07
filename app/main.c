#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/command.h"
#include "../include/parse_args.h"

char *getStdOutFileName(char **pString, int count);

int main() {
    // Flush after every printf
    setbuf(stdout, NULL);
    char* path = getenv("PATH");
    setPath(path);
    while(1) {
        printf("$ ");
        // Wait for user input
        int inputSize = 200;
        char input[inputSize];
        int args_count = 0;
        char *command;
        fgets(input, inputSize, stdin);
        char **args = parse_cmdline(input, &args_count);
        command = args[0];
        for (int i = 0; command[i]; i++) {
            command[i] = tolower(command[i]);
        }
        char* stdOutFile = getStdOutFileName(args, args_count);
        reloadPathCommands();
        if (isValidCommand(command)) {
            FILE *file = NULL;
            if(stdOutFile != NULL) {
                char cmd[1024];
                char *folder = strdup(stdOutFile);
                char *last_slash = strrchr(folder, '/');
                if (last_slash != NULL) {
                    *last_slash = '\0';
                }
                snprintf(cmd, sizeof(cmd), "mkdir -p '%s'", folder);
                system(cmd);
                file = fopen(stdOutFile, "w");
                if (file == NULL) {
                    printf("Error opening file!\n");
                    exit(1);
                }
            }
            processCommand(command, args, args_count, file);
            if(file != NULL) {
                fclose(file);
            }
        } else {
          printf("%s: command not found\n", command);
        }
        free_args(args, args_count);
    }
    return 0;
}

char *getStdOutFileName(char **pString, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(pString[i], ">") == 0 || strcmp(pString[i], "1>") == 0) {
            if (i + 1 < count) {
                return pString[i + 1];
            }
        }
    }
    return NULL;
}
