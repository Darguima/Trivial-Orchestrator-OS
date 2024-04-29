#ifndef FCFS_POLICY_H
#define FCFS_POLICY_H

typedef void* Element;

typedef struct fcfs_queue *FCFSQueue;

FCFSQueue create_fcfs_queue();

int enqueue_fcfs(void* fcfs_queue, Element element);

Element dequeue_fcfs(void* fcfs_queue);

#endif
