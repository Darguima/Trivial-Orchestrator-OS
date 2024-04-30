#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#include "../../datapipe/globals.h"
#include "fifo/create.h"

int main() {
  create_fifo();
  // read from the server FIFO
  int s_fd = open(S_FIFO_PATH, O_RDONLY, 0600);
  if (s_fd == -1) {
      perror("open");
      return 1;
  }

  char* buffer = malloc(sizeof(char) * MAX_BUF_SIZE);
  ssize_t read_bytes;

  while ((read_bytes = read(s_fd, buffer, MAX_BUF_SIZE)) > 0) {
      if (write(STDOUT_FILENO, buffer, read_bytes) == -1) {
        perror("write");
        return 1;
      }
      // TODO: add command interpreter here and execute the command
  }

  // on server close delete the FIFO
  delete_fifo();

  close(s_fd);
  free(buffer);
  return 0;
}