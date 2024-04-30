#include "scheduler/scheduler.h"
#include "stdio.h"
#include "stdlib.h"

int main() {
  Scheduler scheduler = create_scheduler(FCFS);

  char command[100];
  int process_id;
  Process process;

  for (int i = 0; i < 14; i++) {
    sprintf(command, "ls %d", i);
    process_id = enqueue_process(scheduler, command, 10);
    printf("Enqueued Process %d: %s\n", process_id, command);
  }

  for (int i = 0; i < 4; i++) {
    process = dequeue_process(scheduler);
    printf("Dequeued Process %d: %s\n", process->id, process->command);
    free(process->command);
    free(process);
  }

  destroy_scheduler(scheduler);

  return 0;
 }
/*
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#include "../../datapipe/globals.h"
#include "fifo/create.h"
#include "commands/command_interpreter.h"
#include "commands/command_runner.h"

void print_received_command(char* buffer, ssize_t read_bytes) {
    // write on server terminal (just for testing)
    char* output = (char*)malloc(sizeof(char) * (read_bytes + 11));
    if (output == NULL) {
        perror("malloc");
        return;
    }
    strcpy(output, "Received: ");
    strcat(output, buffer);
    printf("%s", output);
    free(output);
}

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
        //print_received_command(buffer, read_bytes);
        // command interpreter and execute the command
        char** command_args = command_interpreter(buffer);
        execute_command(command_args[0], command_args+1);
    }

    // on server close delete the FIFO
    delete_fifo();

    close(s_fd);
    free(buffer);
    return 0;
}*/