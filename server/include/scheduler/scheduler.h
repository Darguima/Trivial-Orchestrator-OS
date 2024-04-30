#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <sys/time.h>

typedef struct process {
  int id;
  char* command;
  int estimated_runtime;
  struct timeval start_time;
  int process_id;
}* Process;

typedef enum schedule_policy {
  FCFS,
  SJF,
} SchedulePolicy;

typedef struct scheduler* Scheduler;

Scheduler create_scheduler(SchedulePolicy schedule_policy);

void destroy_scheduler(Scheduler scheduler);

int enqueue_process(Scheduler scheduler, char* command, int estimated_runtime);

Process dequeue_process(Scheduler scheduler);

#endif
