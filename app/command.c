#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "../include/debug.h"
#include "../include/command.h"
#include "../include/array_list.h"

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 1024

// built-in commands
char COMMANDS[5][10] = {
        "exit",
        "echo",
        "type",
        "pwd",
        "cd"
};
ArrayList *PATH_COMMANDS;
int NUMBER_OF_SUPPORTED_COMMANDS = sizeof(COMMANDS) / sizeof(COMMANDS[0]);
char* PATH_DELIMITERS = ":";
char* PATH_ENV = "/usr/bin";

void executeCommand(char *path, char* args[], FILE* outputFile);

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
    } else if (strcasecmp(command, "echo") == 0) {
        if (argsCount < 2) {
            printf("invalid number of arguments\n");
            return 0;
        }
    } else if (strcasecmp(command, "type") == 0) {
        if (argsCount != 2) {
            printf("invalid number of arguments\n");
            return 0;
        }
    } else if (strcasecmp(command, "pwd") == 0) {
        if (argsCount > 1) {
            printf("invalid number of arguments\n");
            return 0;
        }
    } else if (strcasecmp(command, "cd") == 0) {
        if (argsCount != 2) {
            printf("invalid number of arguments\n");
            return 0;
        }
    }
    return 1;
}

int processCommand(char* command, char* args[], int argsCount, FILE* outputFile) {
    reloadPathCommands();
    if (strcasecmp(command, "exit") == 0) {
        if (validateArguments(command, args, argsCount)) {
            exit(atoi(args[1]));
        }
    } else if (strcasecmp(command, "pwd") == 0) {
        if (validateArguments(command, args, argsCount)) {
            char cwd[1000];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                if(outputFile != NULL) {
                    fprintf(outputFile, "%s\n", cwd);
                } else {
                    printf("%s\n", cwd);
                }
            }
        }
    }  else if (strcasecmp(command, "cd") == 0) {
        if (validateArguments(command, args, argsCount)) {
            char *home_dir = getenv("HOME");
            char *new_dir = args[1];
            if (strcmp(args[1], "~") == 0) {
                new_dir = home_dir;
            }
            if (chdir(new_dir) != 0) {
                if(outputFile != NULL) {
                    fprintf(outputFile, "cd: %s: No such file or directory\n", args[1]);
                } else {
                    printf("cd: %s: No such file or directory\n", args[1]);
                }
            }
        }
    } else if (strcasecmp(command, "echo") == 0) {
        if (validateArguments(command, args, argsCount)) {
            for (int i = 1; i < argsCount; i++) {
                if(outputFile != NULL) {
                    fprintf(outputFile, "%s ", args[i]);
                } else {
                    printf("%s ", args[i]);
                }
            }
            if(outputFile != NULL) {
                fprintf(outputFile, "\n");
            } else {
                printf("\n");
            }
            return 1;
        }
    } else if(strcasecmp(command, "type") == 0) {
        if (validateArguments(command, args, argsCount)) {
            for(int i = 0; i < NUMBER_OF_SUPPORTED_COMMANDS; i++) {
                if (strcasecmp(args[1], COMMANDS[i]) == 0) {
                    if(outputFile != NULL) {
                        fprintf(outputFile, "%s is a shell builtin\n", args[1]);
                    } else {
                        printf("%s is a shell builtin\n", args[1]);
                    }
                    return 1;
                }
            }
            for(int i = 0; i < PATH_COMMANDS->size; i++) {
                char *temp = malloc(strlen(PATH_COMMANDS->data[i]) + 1);
                strcpy(temp, PATH_COMMANDS->data[i]);
                char *path_cmd = strtok(temp, PATH_DELIMITERS);
                char *path = strtok(NULL, PATH_DELIMITERS);
                if (strcasecmp(args[1], path_cmd) == 0) {
                    if(outputFile != NULL) {
                        fprintf(outputFile, "%s is %s\n", path_cmd, path);
                    } else {
                        printf("%s is %s\n", path_cmd, path);
                    }
                    free(temp);
                    return 1;
                }
                free(temp);
            }
            if(outputFile != NULL) {
                fprintf(outputFile, "%s: not found\n", args[1]);
            } else {
                printf("%s: not found\n", args[1]);
            }
        }
    } else {
//        printf("%s: trying to run\n", command);
        for(int i = 0; i < PATH_COMMANDS->size; i++) {
            char *temp = malloc(strlen(PATH_COMMANDS->data[i]) + 1);
            strcpy(temp, PATH_COMMANDS->data[i]);
            char *path_cmd = strtok(temp, PATH_DELIMITERS);
            char *path = strtok(NULL, PATH_DELIMITERS);
            if (strcasecmp(args[0], path_cmd) == 0) {
//                printf("%s is %s\n", path_cmd, path);
                executeCommand(path, args, outputFile);
                free(temp);
                return 1;
            }
            free(temp);
        }
    }
    return 0;
}


char** createNullTerminatedArray(char *arr[], size_t count) {
    // Allocate an extra element for the NULL terminator
    char **new_arr = malloc(sizeof(char *) * (count + 1));
    if (!new_arr) {
        return NULL; // handle allocation failure
    }

    // Copy pointers from arr
    for (size_t i = 0; i < count; i++) {
        new_arr[i] = arr[i];
    }

    // Null-terminate
    new_arr[count] = NULL;

    return new_arr;
}

void executeCommand(char *path, char* args[], FILE* outputFile) {
    int pipe_fd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];

    // Create pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        // Close read end of pipe
        close(pipe_fd[READ_END]);

        // Redirect stdout to pipe
        dup2(pipe_fd[WRITE_END], STDOUT_FILENO);
        close(pipe_fd[WRITE_END]);

        // Execute command
        int args_size = 0;
        while (args[args_size] != NULL) {
            args_size++;
        }
        char **null_terminated_args = createNullTerminatedArray(args, args_size);
        execvp(path, null_terminated_args);

        // If execlp returns, there was an error
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {  // Parent process
        // Close write end of pipe
        close(pipe_fd[WRITE_END]);

        // Read output from child process
        ssize_t bytes_read;
        while ((bytes_read = read(pipe_fd[READ_END], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0';
            if (outputFile != NULL) {
                fprintf(outputFile, "%s", buffer);
            } else {
                printf("%s", buffer);
            }
        }

        // Close read end of pipe
        close(pipe_fd[READ_END]);

        // Wait for child process to finish
        wait(NULL);
    }
}

int reloadPathCommands() {
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
    free(path);
    return 0;
}
