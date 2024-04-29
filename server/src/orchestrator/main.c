#include "orchestrator.h"

int main() {
  Orchestrator orch = create_orchestrator(FCFS);

  for (int i = 0; i < 14; i++) {
    enqueue_process(orch, "ls", 10);
  }

  for (int i = 0; i < 4; i++) {
    dequeue_process(orch);
  }

  for (int i = 0; i < 12; i++) {
    enqueue_process(orch, "ls", 10);
  }

  // for (int i = 0; i < 2; i++) {
  //   dequeue_process(orch);
  // }


  return 0;
}