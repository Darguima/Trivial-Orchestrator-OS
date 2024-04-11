#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands/command_interpreter.h"

char* askforcommand() {
    char* command = (char*)malloc(MAX_COMMAND_LENGTH * sizeof(char));
    printf("Enter a command: \n >> ");
    if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL) {
        perror("Error reading command\n");
        free(command);
        return NULL;
    }
    return command;
}

char** command_interpreter(char* command) {
    char** command_args = (char**)malloc(MAX_COMMAND_LENGTH * sizeof(char*));
    int i = 0;
    int in_quotes = 0;
    size_t arg_start = 0;
    size_t command_length = strlen(command);
    for (size_t j = 0; j <= command_length; j++) {
        if (command[j] == '"' && (j == 0 || command[j - 1] != '\\')) {
            in_quotes = !in_quotes;
        } else if ((command[j] == ' ' && !in_quotes) || command[j] == '\0') {
            size_t arg_length = j - arg_start;
            command_args[i] = (char*)malloc((arg_length + 1) * sizeof(char));
            strncpy(command_args[i], command + arg_start, arg_length);
            command_args[i][arg_length] = '\0'; // Null-terminate the string
            arg_start = j + 1; // Next argument starts after the space
            i++;
        }
    }

    // Remove the quotes from the arguments
    for (int j = 0; j < i; j++) {
        size_t arg_length = strlen(command_args[j]);
        if (command_args[j][0] == '"' && command_args[j][arg_length - 1] == '"') {
            command_args[j]++;
            command_args[j][arg_length - 2] = '\0';
        }
    }

    // Remove unwanted characters from the arguments (like "\n", "\r", etc.)
    for (int j = 0; j < i; j++) {
        size_t arg_length = strlen(command_args[j]);
        for (size_t k = 0; k < arg_length; k++) {
            if (command_args[j][k] == '\n' || command_args[j][k] == '\r') {
                command_args[j][k] = '\0';
            }
        }
    }

    // Free memory and set extra pointers to NULL
    while (i < MAX_COMMAND_LENGTH) {
        command_args[i] = NULL;
        i++;
    }

    return command_args;
}