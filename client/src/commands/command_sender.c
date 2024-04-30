#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "commands/command_sender.h"

#define BUF_SIZE 1024
#define MAX_COMMAND_LENGTH 100

int askforcommand() {
    // open the server FIFO
    int s_fd = open(S_FIFO_PATH, O_WRONLY, 0600);
    if (s_fd == -1) {
        perror("open");
        return 1;
    }

    char* buffer = malloc(sizeof(char) * BUF_SIZE);
    ssize_t read_bytes;

    // write a "Enter a command: \n >> " message that doesnt get sent to the server
    if (write(STDOUT_FILENO, "Enter a command: \n >> ", 22) == -1) {
        perror("write");
        return 1;
    }

    while ((read_bytes = read(STDIN_FILENO, buffer, BUF_SIZE)) > 0) {
        if (write(STDOUT_FILENO, "Enter a command: \n >> ", 22) == -1) {
            perror("write");
            return 1;
        }
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
    // open the server FIFO
    int s_fd = open(S_FIFO_PATH, O_WRONLY, 0600);
    if (s_fd == -1) {
        perror("open");
        return 1;
    }

    // write the command to the server FIFO
    if (write(s_fd, command, strlen(command)) == -1) {
        perror("write");
        return 1;
    }

    close(s_fd);
    return 0;
}