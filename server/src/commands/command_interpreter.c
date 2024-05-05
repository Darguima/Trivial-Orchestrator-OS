#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "scheduler/scheduler.h"


#include "../../datapipe/globals.h"
#include "commands/command_interpreter.h"

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

char* get_command (char* command) {

char* buffer = strdup(command); // Creating a copy of the command
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    char* token;
    char* result = malloc(sizeof(char) * MAX_BUF_SIZE);
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(buffer);
        return NULL;
    }

    // execute 
    token = strtok(buffer, " ");
    
    // time 
    token = strtok(NULL, " ");
    
    // -u// -p
    token = strtok(NULL, " ");

    token = strtok(NULL, ""); 
    
    if (token) {
        strncpy(result, token, MAX_BUF_SIZE - 1);  // Copies the command to the result string
        result[MAX_BUF_SIZE - 1] = '\0'; // Null-terminates the string
    } else {
        *result = '\0'; // Empty string
    }

    free(buffer); // Free the buffer
    return result;
}



void status_writer(char* client_fifo_path,Process processes[MAX_SIMULTANEOUS],int active_tasks) {
    if (fork () == 0 ) {
     char* buffer = malloc(sizeof(char) * MAX_BUF_SIZE);
    if (buffer == NULL) {
        perror("Failed to allocate memory for buffer");
        exit(EXIT_FAILURE);
    }

    int client_fifo_fd = open(client_fifo_path, O_WRONLY);
    if (client_fifo_fd == -1) {
        perror("Error opening client FIFO");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    // Send the header
    snprintf(buffer, MAX_BUF_SIZE, "Executing\n");
    write(client_fifo_fd, buffer, strlen(buffer));
    //memset(buffer, 0, MAX_BUF_SIZE);

    // Send the processes in execution
    for (int i = 0; i < active_tasks; i++) {
       char*  command = get_command(processes[i]->command);
        if (command == NULL) {
            perror("Failed to get command");
            memset(buffer, 0, MAX_BUF_SIZE);
            continue;
        }
        snprintf(buffer, MAX_BUF_SIZE, "Task ID = %d %s\n", processes[i]->id, command);
        write(client_fifo_fd, buffer, strlen(buffer));
        memset(buffer, 0, MAX_BUF_SIZE);
        free(command);
        
    }
    free(buffer);

     char* buffer_2 = malloc(sizeof(char) * MAX_BUF_SIZE);
    if (buffer_2 == NULL) {
        perror("Failed to allocate memory for buffer");
        exit(EXIT_FAILURE);
    }


    snprintf(buffer_2, MAX_BUF_SIZE, "Finished\n");
    write(client_fifo_fd, buffer_2, strlen(buffer_2));

    int log_fd = open(LOG_PATH, O_RDONLY);
    if (log_fd == -1) {
        perror("Error opening log file");
        free(buffer_2);
        exit(EXIT_FAILURE);
    }

    while (read(log_fd, buffer_2, MAX_BUF_SIZE) > 0) {
        write(client_fifo_fd, buffer_2, strlen(buffer_2));
        memset(buffer_2, 0, MAX_BUF_SIZE);
    }

    close(log_fd);
    close(client_fifo_fd);
    free(buffer_2);
      _exit(0);
    }
   
}

char** parse_command(const char* command) {
    printf ("[DEBUG] - Parsing command: %s\n", command);
    // Creates a copy of the command string
    char* buffer = strdup(command);
    if (buffer == NULL) {
        printf("Failed to duplicate command string");
    }

    int arg_count = 0;
    char** args = NULL;
    char* token;
    const char* delim = " ";

    
    strtok(buffer, delim); // "execute"
    strtok(NULL, delim);   // "time"
    strtok(NULL, delim);   // "-u"

    // Process the remaining arguments
    while ((token = strtok(NULL, delim)) != NULL) {
        // Realocates the array of arguments
        char** new_args = realloc(args, (arg_count + 2) * sizeof(char*)); // +2 para novo arg e NULL
        if (new_args == NULL) {
           printf("Failed to reallocate arguments array");
            free(args); // Free the old array
            free(buffer); // Free the buffer
            return NULL;
        }
        args = new_args;
        args[arg_count++] = strdup(token);
    }
    if (args != NULL) {
        args[arg_count] = NULL;  // Last argument is NULL
    }

    free(buffer); // Free the buffer
    return args;
}


