#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/command.h"

int main() {
    // Flush after every printf
    setbuf(stdout, NULL);
    char* path = getenv("PATH");
    setPath(path);
    while(1) {
        // Uncomment this block to pass the first stage
        printf("$ ");
        // Wait for user input
        char input[100];
        char *token;
        char *delimiters = " \t\n";
        int argsCount = 0;
        char *args[10]; // Array to command line arguments
        char *command;
        fgets(input, 100, stdin);
        token = strtok(input, delimiters);
        while (token != NULL) {
            args[argsCount] = token;
            argsCount++;
            token = strtok(NULL, delimiters);
        }
        command = args[0];
        for (int i = 0; command[i]; i++) {
            command[i] = tolower(command[i]);
        }
        reloadPathCommands();
        if (isValidCommand(command)) {
          processCommand(command, args, argsCount);
        } else {
          printf("%s: command not found\n", command);
        }
    }
    return 0;
}
