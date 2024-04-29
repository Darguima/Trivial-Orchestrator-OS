#include "scheduler/scheduler.h"
#include "scheduler/fcfs_policy.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct queue* Queue;
typedef int (*EnqueueFunction)(Queue, Process);
typedef Process (*DequeueFunction)(Queue);

typedef struct scheduler {
  SchedulePolicy policy;

  Queue queue;

  EnqueueFunction enqueue_fun;
  DequeueFunction dequeue_fun;

}* Scheduler;

Scheduler create_scheduler(SchedulePolicy schedule_policy) {
  Scheduler scheduler;
  scheduler = malloc(sizeof(struct scheduler));

  scheduler->policy = schedule_policy;

  if (scheduler->policy == FCFS) {
    scheduler->queue = (Queue)create_fcfs_queue();
    scheduler->enqueue_fun = (EnqueueFunction)enqueue_fcfs;
    scheduler->dequeue_fun = (DequeueFunction)dequeue_fcfs;
  }

  printf("[LOG] - Scheduler created with policy: %s;\n", schedule_policy == FCFS ? "FCFS" : "None");

  return scheduler;
}

int enqueue_process(Scheduler scheduler, char* command, int estimated_runtime) {
  Process new_process = malloc(sizeof(struct process));

  new_process->command = strdup(command);
  new_process->estimated_runtime = estimated_runtime;

  new_process->id = scheduler->enqueue_fun(scheduler->queue, new_process);

  return new_process->id;
}

Process dequeue_process(Scheduler scheduler) {
  return scheduler->dequeue_fun(scheduler->queue);
}