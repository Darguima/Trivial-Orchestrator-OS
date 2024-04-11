#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "commands/command_runner.h"

void execute_command(char* command_name, char** command_args) {
    // TODO: add time, etc. (right now its just a simple command executer)
    if (fork() == 0) {
        char command_path[100];
        snprintf(command_path, sizeof(command_path), "/bin/%s", command_name);
        // if not found in /bin use the project commands folder
        if (access(command_path, X_OK) == -1) {
            snprintf(command_path, sizeof(command_path), "commands/%s", command_name);
        }
        execvp(command_path, command_args);
        perror("Error executing command");
        exit(1);
    }
}