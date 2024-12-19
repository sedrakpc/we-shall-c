#include <stdio.h>
#include <string.h>
#include "../include/command.h"

int main() {
  // Flush after every printf
  setbuf(stdout, NULL);

  // Uncomment this block to pass the first stage
   printf("$ ");

  // Wait for user input
  while(1) {
      char input[100];
      fgets(input, 100, stdin);
      int len = strlen(input);
      if (len > 0 && input[len - 1] == '\n') {
          input[len - 1] = '\0';
      }
      if (isValidCommand(input)) {
          printf("Command is valid\n");
      } else {
          printf("%s: command not found\n", input);
      }
  }
  return 0;
}
