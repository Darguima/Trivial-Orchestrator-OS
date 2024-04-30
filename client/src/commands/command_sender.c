#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "../../datapipe/globals.h"
#include "commands/command_sender.h"

void asktext() {
    if (write(STDOUT_FILENO, "Enter a command: \n >> ", 22) == -1) { // write a message that doesnt get sent to the server
        perror("write");
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