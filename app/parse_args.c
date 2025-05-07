#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "../include/parse_args.h"

#define MAX_ARGS 64
#define MAX_ARG_LEN 256

char **parse_cmdline(const char *cmdline, int *argc) {
    char **argv = malloc(MAX_ARGS * sizeof(char *));
    for (int i = 0; i < MAX_ARGS; i++) {
        argv[i] = malloc(MAX_ARG_LEN * sizeof(char));
    }

    *argc = 0;
    int arg_pos = 0;
    bool in_quotes = false;
    bool escaped = false;
    char quote_char = '\0';

    for (int i = 0; cmdline[i] != '\0'; i++) {
        char c = cmdline[i];

        // Handle escape character
        if (c == '\\' && !escaped) {
            if (!in_quotes || (in_quotes && quote_char == '"' &&
                               (cmdline[i + 1] == '\\' || cmdline[i + 1] == '$' ||
                                cmdline[i + 1] == '"' || cmdline[i + 1] == '\n'))) {
                escaped = true;
                continue;
            }
        }

        // Handle quotes
        if ((c == '"' || c == '\'') && !escaped) {
            if (!in_quotes) {
                in_quotes = true;
                quote_char = c;
                continue;
            } else if (c == quote_char) {
                in_quotes = false;
                quote_char = '\0';
                continue;
            }
        }

        // Handle spaces
        if (isspace(c) && !in_quotes && !escaped) {
            if (arg_pos > 0) {
                argv[*argc][arg_pos] = '\0';
                (*argc)++;
                arg_pos = 0;

                if (*argc >= MAX_ARGS - 1) {
                    break;
                }
            }
            continue;
        }

        // Add character to current argument
        if (arg_pos < MAX_ARG_LEN - 1) {
            if (escaped) {
                if (!in_quotes || (in_quotes && quote_char == '"' &&
                                   (c == '\\' || c == '$' || c == '"' || c == '\n'))) {
                    argv[*argc][arg_pos++] = c;
                } else {
                    argv[*argc][arg_pos++] = '\\';
                    argv[*argc][arg_pos++] = c;
                }
                escaped = false;
            } else {
                argv[*argc][arg_pos++] = c;
            }
        }
    }

    // Add final argument if exists
    if (arg_pos > 0) {
        argv[*argc][arg_pos] = '\0';
        (*argc)++;
    }

    // Null terminate argument list
    argv[*argc] = NULL;

    return argv;
}

void free_args(char **argv, int argc) {
    for (int i = 0; i < argc; i++) {
        free(argv[i]);
    }
    free(argv);
}