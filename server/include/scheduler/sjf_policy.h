#ifndef SJF_POLICY_H
#define SJF_POLICY_H

typedef void* Element;

typedef struct sjf_queue* SJFQueue;

#include "process.h"

SJFQueue create_sjf_queue();

void destroy_sjf(SJFQueue sjf_queue);

int enqueue_sjf(SJFQueue sjf_queue, Process process);

Element dequeue_sjf(SJFQueue sjf_queue);

#endif
