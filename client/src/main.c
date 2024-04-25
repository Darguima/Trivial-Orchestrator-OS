#include "hello_world/hello_world.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define SERVER_FIFO "../tmp/fifo_server"

int main() {
  int server_fd, client_fd;
  char client_fifo[256], buffer[300];
  sprintf(client_fifo, "../tmp/fifo_client_%d", getpid());

  if (mkfifo(client_fifo, 0666) == -1) {
    perror("Error creating client FIFO");
    exit(EXIT_FAILURE);
  }

  server_fd = open(SERVER_FIFO, O_WRONLY);
  if (server_fd == -1) {
    perror("Erro ao abrir FIFO do servidor");
    unlink(client_fifo);
    exit(EXIT_FAILURE);
  }

  sprintf(buffer, "%s %s", client_fifo, "execute");
  size_t k = write(server_fd, buffer, strlen(buffer) + 1);
  close(server_fd);

  client_fd = open(client_fifo, O_RDONLY);
  if (client_fd == -1) {
    perror("Error opening client FIFO");
    unlink(client_fifo);
    exit(EXIT_FAILURE);
  }
   
  if ( (int) read(client_fd, buffer, sizeof(buffer)) > 0) {
    printf("ID of the task: %s\n", buffer);
  }

  close(client_fd);
  unlink(client_fifo);

  return EXIT_SUCCESS;
}
