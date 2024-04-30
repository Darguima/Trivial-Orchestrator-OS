#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define SERVER_FIFO "../tmp/fifo_server"


int main() {
    int server_fd, client_fd;
    char client_fifo[256], buffer[1024];
    int client_pid = getpid();

    // Create client FIFO name based on PID
    sprintf(client_fifo, "../tmp/fifo_client_%d", client_pid);

    // Create client FIFO
    if (mkfifo(client_fifo, 0666) == -1) {
        perror("Error creating client FIFO");
        exit(EXIT_FAILURE);
    }

    // Open server FIFO for writing
    server_fd = open(SERVER_FIFO, O_WRONLY);
    if (server_fd == -1) {
        perror("Erro ao abrir FIFO do servidor");
        unlink(client_fifo);
        exit(EXIT_FAILURE);
    }

    // sending client  pid to the server
    sprintf(buffer, "%s", client_fifo);
    if (write(server_fd, buffer, strlen(buffer) + 1) == -1) {
        perror("Failed to write to server FIFO");
    }
    close(server_fd);

    // Open client FIFO for reading
    client_fd = open(client_fifo, O_RDONLY);
    if (client_fd == -1) {
        perror("Error opening client FIFO");
        unlink(client_fifo);
        exit(EXIT_FAILURE);
    }

    // Read response from server
    if (read(client_fd, buffer, sizeof(buffer)) > 0) {
        printf("ID of the task: %s\n", buffer);
    } else {
        perror("Failed to read from client FIFO");
    }

    // Close and remove client FIFO
    close(client_fd);
    // 

    unlink(client_fifo);

    return EXIT_SUCCESS;
}
