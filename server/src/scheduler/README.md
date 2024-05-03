# Scheduler

The scheduler will store all processes ready to run on a queue and, when requested, choose one based on a Scheduling Policy.

## The code

## Scheduler API

Here is a simple example of how to use the scheduler:

```c
#include "scheduler/scheduler.h"

int main() {
  Scheduler scheduler = create_scheduler(FCFS);

  enqueue_process(scheduler, "ls /etc/ -la", 10);

  Process p = dequeue_process(scheduler);

  /*
    p->id == 1
    p->command == "ls /etc/ -la"
    p->estimated_runtime == 10
  */

  free(p);

  destroy_scheduler(scheduler);

  return 0;
}
```

## How create new policies

All the code is encapsulated, so you just need to create a new policy file with the following functions:

```c
// replace policy with the policy name

// This are the elements that will be stored in the queue
typedef void* Element;

// This is a structure that can store everything that you need
typedef struct policy_queue* PolicyQueue;

Queue create_policy_queue();

// free all the memory allocated by the queue
void destroy_fcfs(FCFSQueue fcfs_queue);

int enqueue_policy(Queue policy_queue, Element element);

Element dequeue_policy(Queue policy_queue);
```

Then you just need add the policy to the `scheduler.c` file:

```c
// don't forget to add YOUR_POLICY to the SchedulePolicy enum

if (scheduler->policy == YOUR_POLICY) {
  scheduler->queue = (Queue)create_policy_queue();
  scheduler->destroy_fun = (DestroyFunction)destroy_policy;
    
  scheduler->enqueue_fun = (EnqueueFunction)enqueue_policy;
  scheduler->dequeue_fun = (DequeueFunction)dequeue_policy;
}
```

## Policies

### FCFS (First Come First Served)

The first process to arrive will be the first to be executed.

This module use a circular queue to store the processes, reusing the unqueued positions. When the queue is full, it will be resized to double of its size, and move all processes, in the same order, to the start of the new queue.

Here is a visualization of multiple enqueue and dequeue operations:

```plaintext
===================================================================
[DEBUG] - Created FCFS queue with 16 elements capacity;
[DEBUG] - Orchestrator created with policy: FCFS;
  ENQUEUED: [ x  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _ ]
  ENQUEUED: [ x  x  _  _  _  _  _  _  _  _  _  _  _  _  _  _ ]
  ENQUEUED: [ x  x  x  _  _  _  _  _  _  _  _  _  _  _  _  _ ]
  ENQUEUED: [ x  x  x  x  _  _  _  _  _  _  _  _  _  _  _  _ ]
  ENQUEUED: [ x  x  x  x  x  _  _  _  _  _  _  _  _  _  _  _ ]
  ENQUEUED: [ x  x  x  x  x  x  _  _  _  _  _  _  _  _  _  _ ]
  ENQUEUED: [ x  x  x  x  x  x  x  _  _  _  _  _  _  _  _  _ ]
  ENQUEUED: [ x  x  x  x  x  x  x  x  _  _  _  _  _  _  _  _ ]
  ENQUEUED: [ x  x  x  x  x  x  x  x  x  _  _  _  _  _  _  _ ]
  ENQUEUED: [ x  x  x  x  x  x  x  x  x  x  _  _  _  _  _  _ ]
  ENQUEUED: [ x  x  x  x  x  x  x  x  x  x  x  _  _  _  _  _ ]
  ENQUEUED: [ x  x  x  x  x  x  x  x  x  x  x  x  _  _  _  _ ]
  ENQUEUED: [ x  x  x  x  x  x  x  x  x  x  x  x  x  _  _  _ ]
  ENQUEUED: [ x  x  x  x  x  x  x  x  x  x  x  x  x  x  _  _ ]
   DEQUEUE: [ _  x  x  x  x  x  x  x  x  x  x  x  x  x  _  _ ]
   DEQUEUE: [ _  _  x  x  x  x  x  x  x  x  x  x  x  x  _  _ ]
   DEQUEUE: [ _  _  _  x  x  x  x  x  x  x  x  x  x  x  _  _ ]
   DEQUEUE: [ _  _  _  _  x  x  x  x  x  x  x  x  x  x  _  _ ]
  ENQUEUED: [ _  _  _  _  x  x  x  x  x  x  x  x  x  x  x  _ ]
  ENQUEUED: [ _  _  _  _  x  x  x  x  x  x  x  x  x  x  x  x ]
  ENQUEUED: [ x  _  _  _  x  x  x  x  x  x  x  x  x  x  x  x ]
  ENQUEUED: [ x  x  _  _  x  x  x  x  x  x  x  x  x  x  x  x ]
  ENQUEUED: [ x  x  x  _  x  x  x  x  x  x  x  x  x  x  x  x ]
   DEQUEUE: [ x  x  x  _  _  x  x  x  x  x  x  x  x  x  x  x ]
  ENQUEUED: [ x  x  x  x  _  x  x  x  x  x  x  x  x  x  x  x ]
  ENQUEUED: [ x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x ]
   RESIZED: [ x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _ ]
  ENQUEUED: [ x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  _  _  _  _  _  _  _  _  _  _  _  _  _  _  _ ]
  ENQUEUED: [ x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  _  _  _  _  _  _  _  _  _  _  _  _  _  _ ]
  ENQUEUED: [ x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  _  _  _  _  _  _  _  _  _  _  _  _  _ ]
  ENQUEUED: [ x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  _  _  _  _  _  _  _  _  _  _  _  _ ]
   DEQUEUE: [ _  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  _  _  _  _  _  _  _  _  _  _  _  _ ]
   DEQUEUE: [ _  _  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  _  _  _  _  _  _  _  _  _  _  _  _ ]
   DEQUEUE: [ _  _  _  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  _  _  _  _  _  _  _  _  _  _  _  _ ]
   DEQUEUE: [ _  _  _  _  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  _  _  _  _  _  _  _  _  _  _  _  _ ]
===================================================================
```