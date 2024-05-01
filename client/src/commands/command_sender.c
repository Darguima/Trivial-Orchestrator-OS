#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "../../datapipe/globals.h"
#include "commands/command_sender.h"

#define TERMINAL_YELLOW "\033[0;33m"
#define TERMINAL_RESET "\033[0m"

void asktext() {
    const char* prompt = TERMINAL_YELLOW "Enter a command:\n >> " TERMINAL_RESET;
    if (write(STDOUT_FILENO, prompt, strlen(prompt)) == -1) {
        perror("write");
        exit(1);
    }
}

int askforcommand() {
    int s_fd = open(S_FIFO_PATH, O_WRONLY, 0600); // open the server FIFO
    if (s_fd == -1) {
        perror("open");
        return 1;
    }

    char* buffer = malloc(sizeof(char) * MAX_BUF_SIZE);
    ssize_t read_bytes;

    asktext();
    while ((read_bytes = read(STDIN_FILENO, buffer, MAX_BUF_SIZE)) > 0) {
        asktext();
        if (write(s_fd, buffer, read_bytes) == -1) {
            perror("write");
            return 1;
        }
    }

    close(s_fd);
    free(buffer);
    return 0;
}

int send_command(char* command) {
    int s_fd = open(S_FIFO_PATH, O_WRONLY, 0600); // open the server FIFO
    if (s_fd == -1) {
        perror("open");
        return 1;
    }

    if (write(s_fd, command, strlen(command)) == -1) { // write the command to the server FIFO
        perror("write");
        return 1;
    }

    close(s_fd);
    return 0;
}

void send_create_task_fifo(char* predicted_c_fifo) {
    char* create_command = (char*)malloc(MAX_COMMAND_LENGTH * sizeof(char));
    sprintf(create_command, "create_taskuser %s", predicted_c_fifo);
    send_command(create_command);
    free(create_command);
}

void send_delete_task_fifo(char* predicted_c_fifo) {
    char* delete_command = (char*)malloc(MAX_COMMAND_LENGTH * sizeof(char));
    sprintf(delete_command, "delete_taskuser %s", predicted_c_fifo);
    send_command(delete_command);
    free(delete_command);
}