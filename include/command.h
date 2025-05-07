//
// Created by Sedrak Dalaloyan on 12/19/24.
//

#ifndef CODECRAFTERS_SHELL_C_COMMAND_H
#define CODECRAFTERS_SHELL_C_COMMAND_H

void setPath(char *path);

int isValidCommand(char *command);

int processCommand(char *command, char *args[], int argsCount, FILE *outputFile);

int reloadPathCommands();

#endif //CODECRAFTERS_SHELL_C_COMMAND_H
