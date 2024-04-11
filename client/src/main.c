#include <stdio.h>
#include <stdlib.h>

#include "commands/command_interpreter.h"
#include "commands/command_runner.h"

int main(int argc, char** argv) {
  if (argc <= 1) {
    char* command = askforcommand();
    char** command_parts = command_interpreter(command);
    char* command_name = command_parts[0];
    char** command_args = command_parts + 1;
    printf("Command name: %s\n", command_name);
    for (int i = 0; command_args[i] != NULL; i++) {
      printf("Command arg %d: %s\n", i, command_args[i]);
    }
    execute_command(command_name, command_args);
    free(command);
  } else {
    char* command_name = argv[1];
    char** command_args = argv + 2;
    printf("Command name: %s\n", command_name);
    for (int i = 0; command_args[i] != NULL; i++) {
      printf("Command arg %d: %s\n", i, command_args[i]);
    }
    execute_command(command_name, command_args);
  }
  return 0;
}