//
// Created by Sedrak Dalaloyan on 1/19/25.
//

#ifndef CODECRAFTERS_SHELL_C_PARSE_ARGS_H
#define CODECRAFTERS_SHELL_C_PARSE_ARGS_H

char** parse_cmdline(const char* cmdline, int* argc);
void free_args(char** args, int argc);

#endif //CODECRAFTERS_SHELL_C_PARSE_ARGS_H
