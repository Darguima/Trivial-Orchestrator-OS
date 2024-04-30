#include <stdio.h>
#include <stdlib.h>

#include "commands/command_runner.h"

void execute_command(char* command_name, char** command_args) {
    // TODO: add time, etc. (right now its just a simple command executer)
    printf("Executing command: %s\n", command_name);
    for (int i = 0; command_args[i] != NULL; i++) {
        printf("Argument %d: %s\n", i, command_args[i]);
    }
}