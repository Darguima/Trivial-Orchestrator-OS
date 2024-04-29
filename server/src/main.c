#include "scheduler/scheduler.h"

int main() {
  Scheduler scheduler = create_scheduler(FCFS);

  for (int i = 0; i < 14; i++) {
    enqueue_process(scheduler, "ls", 10);
  }

  for (int i = 0; i < 4; i++) {
    dequeue_process(scheduler);
  }

  for (int i = 0; i < 10; i++) {
    enqueue_process(scheduler, "ls", 10);
  }

  for (int i = 0; i < 4; i++) {
    dequeue_process(scheduler);
  }

  return 0;
}