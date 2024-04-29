typedef struct process {
  int id;
  char* command;
  int estimated_runtime;
}* Process;

typedef enum schedule_policy {
  FCFS,
  SJF,
} SchedulePolicy;

typedef struct orchestrator* Orchestrator;

Orchestrator create_orchestrator(SchedulePolicy schedule_policy);

void enqueue_process(Orchestrator orch, char* command, int estimated_runtime);

Process dequeue_process(Orchestrator orch);
