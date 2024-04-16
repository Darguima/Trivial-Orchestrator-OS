#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "fifo/create.h"

int create_fifo() {
  // Create Client FIFO file
  unlink(C_FIFO_PATH);
  if (mkfifo(C_FIFO_PATH, 0600) == -1) {
      perror("mkfifo");
      return 1;
  }
  // Create Server FIFO file
  unlink(S_FIFO_PATH);
  if (mkfifo(S_FIFO_PATH, 0600) == -1) {
      perror("mkfifo");
      return 1;
  }
  //printf("FIFO files created successfully!\n");
  return 0;
}