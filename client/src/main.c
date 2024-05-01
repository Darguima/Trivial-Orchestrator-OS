#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include "../../datapipe/globals.h"
#include "commands/command_sender.h"

int client_pid;

void signal_handler(int signum) {
  if (signum == SIGINT || signum == SIGTERM) {
    send_delete_task_fifo(client_pid);
    exit(EXIT_SUCCESS);
  }
}

int main(int argc, char** argv) {
  signal(SIGINT, signal_handler); // CTRL + C
  signal(SIGTERM, signal_handler); // exit

  client_pid = getpid();
  send_create_task_fifo(client_pid);

  if (argc <= 1) {
    ask_for_command(client_pid);
  } else {
    // Command is argv except the first element
    char* command = (char*)malloc(MAX_COMMAND_LENGTH * sizeof(char));
    for (int i = 1; i < argc; i++) {
      strcat(command, argv[i]);
      if (i < argc - 1) {
        strcat(command, " ");
      }
    }
    send_command(client_pid, command);
    free(command);
  }

  // Open client FIFO for reading
  char* predicted_c_fifo = (char*)malloc(MAX_FIFO_NAME * sizeof(char));
  sprintf(predicted_c_fifo, "%s_%d", C_FIFO_PATH, client_pid);
  int client_fd = open(predicted_c_fifo, O_RDONLY);
  if (client_fd == -1) {
    perror("Error opening client FIFO");
    exit(EXIT_FAILURE);
  }

  // Read response from server
  char* buffer = malloc(sizeof(char) * MAX_BUF_SIZE);
  ssize_t read_bytes;
  if ((read_bytes = read(client_fd, buffer, MAX_BUF_SIZE)) > 0) {
    printf("[DEBUG] - ID of the task: %s\n", buffer);
  } else {
    perror("Failed to read from client FIFO");
    exit(EXIT_FAILURE);
  }

  // Close and remove client FIFO
  close(client_fd);
  free(buffer);
  free(predicted_c_fifo);

  return EXIT_SUCCESS;
}