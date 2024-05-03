#include "scheduler/sjf_policy.h"
#include "stdio.h"
#include "stdlib.h"

typedef struct sjf_queue
{
  Element *arr;
  int size;
  int capacity;
  int next_id;
} *SJFQueue;

int get_sjf_queue_length(SJFQueue sjf_queue);

SJFQueue create_sjf_queue()
{
  SJFQueue queue = malloc(sizeof(struct sjf_queue));

  queue->size = 0;
  queue->capacity = 16;
  queue->next_id = 0;

  queue->arr = malloc(sizeof(Element *) * (long unsigned int)queue->capacity);

  printf("[LOG] - Created SJF queue with %d elements capacity;\n", queue->capacity);

  return queue;
}

void destroy_sjf(SJFQueue sjf_queue)
{
  free(sjf_queue->arr);
  free(sjf_queue);

  printf("[LOG] - Destroyed SJF queue;\n");
}

int enqueue_sjf(SJFQueue sjf_queue, Process process)
{
  if (sjf_queue->size == sjf_queue->capacity)
  {

    sjf_queue->capacity *= 2;
    sjf_queue->arr = malloc(sizeof(Element) * (long unsigned int)sjf_queue->capacity);

    printf("[LOG] - Resized SJF queue to %d elements;\n", sjf_queue->capacity);
  }

  int element_id = sjf_queue->next_id;

  sjf_queue->next_id++;
  sjf_queue->arr[sjf_queue->size] = process;
  sjf_queue->size++;

  printf("[LOG] - Enqueued element %d to SJF queue on position %d;\n", element_id, sjf_queue->size);

  return element_id;
}

Element dequeue_sjf(SJFQueue sjf_queue)
{
  if (sjf_queue->size == 0)
  {
    printf("[LOG] - SJF queue is empty;\n");
    return NULL;
  }

  sjf_queue->size--;
  Process process = sjf_queue->arr[sjf_queue->size];

  printf("[LOG] - Dequeued element from SJF queue - remain %d elements;\n", sjf_queue->size);

  return process;
}
