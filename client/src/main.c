#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#include "../../datapipe/globals.h"
#include "fifo/create.h"

int main() {
  char client_fifo[MAX_FIFO_NAME], buffer[MAX_BUF_SIZE];
  int client_pid = getpid();

  // Create client FIFO name based on PID
  sprintf(client_fifo, "%s_%d", C_FIFO_PATH, client_pid);
  create_fifo(client_fifo);

  // Open server FIFO for writing
  int server_fd = open(S_FIFO_PATH, O_WRONLY);
  if (server_fd == -1) {
    perror("Erro ao abrir FIFO do servidor");
    delete_fifo(client_fifo);
    exit(EXIT_FAILURE);
  }

  // Sending client pid to the server
  sprintf(buffer, "%s", client_fifo);
  if (write(server_fd, buffer, strlen(buffer) + 1) == -1) {
    perror("Failed to write to server FIFO");
    delete_fifo(client_fifo);
    exit(EXIT_FAILURE);
  }
  close(server_fd);

  // Open client FIFO for reading
  int client_fd = open(client_fifo, O_RDONLY);
  if (client_fd == -1) {
    perror("Error opening client FIFO");
    delete_fifo(client_fifo);
    exit(EXIT_FAILURE);
  }

  // Read response from server
  if (read(client_fd, buffer, sizeof(buffer)) > 0) {
    printf("ID of the task: %s\n", buffer);
  } else {
    perror("Failed to read from client FIFO");
  }

  // Close and remove client FIFO
  delete_fifo(client_fifo);
  close(client_fd);

  return EXIT_SUCCESS;
}

/*#include <string.h>

#include "../../datapipe/globals.h"
#include "commands/command_sender.h"

int main(int argc, char** argv) {
  if (argc <= 1) {
    askforcommand();
  } else {
    // command is argv except the first element
    char* command = (char*)malloc(MAX_COMMAND_LENGTH * sizeof(char));
    for (int i = 1; i < argc; i++) {
      strcat(command, argv[i]);
      if (i < argc - 1) {
        strcat(command, " ");
      }
    }
    send_command(command);
    free(command);
  }
  return 0;
}*/
