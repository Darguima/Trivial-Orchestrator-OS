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

  return 0;
}