#include "orchestrator.h"
#include "fcfs_policy.h"
#include "stdio.h"
#include "stdlib.h"

typedef void* Queue;

typedef struct orchestrator {
  SchedulePolicy policy;

  Queue queue;

  int (*enqueue_process)(Queue queue, void* process);
  void* (*dequeue_process)(void* queue);
}* Orchestrator;

Orchestrator create_orchestrator(SchedulePolicy schedule_policy) {
  Orchestrator orch;
  orch = malloc(sizeof(struct orchestrator));

  orch->policy = schedule_policy;

  if (orch->policy == FCFS) {
    orch->queue = create_fcfs_queue();
    orch->enqueue_process = enqueue_fcfs;
    orch->dequeue_process = dequeue_fcfs;
  }

  printf("[LOG] - Orchestrator created with policy: %s;\n", schedule_policy == FCFS ? "FCFS" : "None");

  return orch;
}

void enqueue_process(Orchestrator orch, char* command, int estimated_runtime) {
  Process new_process = malloc(sizeof(struct process));

  new_process->command = command;
  new_process->estimated_runtime = estimated_runtime;

  new_process->id = orch->enqueue_process(orch->queue, new_process);
}

Process dequeue_process(Orchestrator orch) {
  return orch->dequeue_process(orch->queue);
}