#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../../datapipe/globals.h"
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

int delete_fifo() {
  // Delete Client FIFO file
  if (unlink (C_FIFO_PATH) == -1) {
      perror("unlink");
      return 1;
  }
  remove(C_FIFO_PATH);

  // Delete Server FIFO file
  if (unlink (S_FIFO_PATH) == -1) {
      perror("unlink");
      return 1;
  }
  remove(S_FIFO_PATH);

  //printf("FIFO files deleted successfully!\n");
  return 0;
}