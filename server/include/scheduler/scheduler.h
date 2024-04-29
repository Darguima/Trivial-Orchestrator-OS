#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct process {
  int id;
  char* command;
  int estimated_runtime;
}* Process;

typedef enum schedule_policy {
  FCFS,
  SJF,
} SchedulePolicy;

typedef struct scheduler* Scheduler;

Scheduler create_scheduler(SchedulePolicy schedule_policy);

int enqueue_process(Scheduler scheduler, char* command, int estimated_runtime);

Process dequeue_process(Scheduler scheduler);

#endif
