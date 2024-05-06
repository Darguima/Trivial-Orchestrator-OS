#include "scheduler/fcfs_policy.h"
#include "stdio.h"
#include "stdlib.h"

typedef struct fcfs_queue
{
  Element *arr;
  int starting_index;
  int ending_index;
  int capacity;
  int next_id;
} *FCFSQueue;

int get_fcfs_queue_length(FCFSQueue fcfs_queue);

FCFSQueue create_fcfs_queue()
{
  FCFSQueue queue = malloc(sizeof(struct fcfs_queue));

  queue->next_id = 0;
  queue->capacity = 16;
  queue->starting_index = 0;
  queue->ending_index = -1;

  queue->arr = malloc(sizeof(Element *) * (long unsigned int)queue->capacity);

  printf("[LOG] - Created FCFS queue with %d elements capacity;\n", queue->capacity);

  return queue;
}

void destroy_fcfs(FCFSQueue fcfs_queue)
{
  free(fcfs_queue->arr);
  free(fcfs_queue);

  printf("[LOG] - Destroyed FCFS queue;\n");
}

int enqueue_fcfs(FCFSQueue fcfs_queue, Element element)
{
  int queue_len = get_fcfs_queue_length(fcfs_queue);

  if (queue_len == fcfs_queue->capacity)
  {
    Element *new_queue = malloc(sizeof(Element) * (long unsigned int)fcfs_queue->capacity * 2);

    int new_queue_index = 0, old_queue_index = fcfs_queue->starting_index;
    do
    {
      new_queue[new_queue_index] = fcfs_queue->arr[old_queue_index];
      old_queue_index = (old_queue_index + 1) % fcfs_queue->capacity;
      new_queue_index++;
    } while (new_queue_index < fcfs_queue->capacity);

    free(fcfs_queue->arr);

    fcfs_queue->starting_index = 0;
    fcfs_queue->ending_index = fcfs_queue->capacity - 1;
    fcfs_queue->arr = new_queue;
    fcfs_queue->capacity *= 2;

    printf("[LOG] - Resized FCFS queue to %d elements;\n", fcfs_queue->capacity);
  }

  int element_id = fcfs_queue->next_id;

  fcfs_queue->next_id++;
  fcfs_queue->ending_index = (fcfs_queue->ending_index + 1) % fcfs_queue->capacity;
  fcfs_queue->arr[fcfs_queue->ending_index] = element;

  printf("[LOG] - Enqueued element %d to FCFS queue on position %d;\n", element_id, fcfs_queue->ending_index);

  return element_id;
}

Element dequeue_fcfs(FCFSQueue fcfs_queue)
{
  if (fcfs_queue->ending_index == -1)
  {
    // printf("[LOG] - FCFS queue is empty;\n");
    return NULL;
  }

  Element element = fcfs_queue->arr[fcfs_queue->starting_index];

  if (fcfs_queue->starting_index == fcfs_queue->ending_index)
  {
    fcfs_queue->starting_index = 0;
    fcfs_queue->ending_index = -1;
  }
  else
  {
    fcfs_queue->starting_index = (fcfs_queue->starting_index + 1) % fcfs_queue->capacity;
  }

  int queue_len = get_fcfs_queue_length(fcfs_queue);
  printf("[LOG] - Dequeued element from FCFS queue - remain %d elements;\n", queue_len);

  return element;
}

int get_fcfs_queue_length(FCFSQueue fcfs_queue)
{
  if (fcfs_queue->ending_index == -1)
    return 0;

  return fcfs_queue->starting_index <= fcfs_queue->ending_index
             ? fcfs_queue->ending_index - fcfs_queue->starting_index + 1
             : fcfs_queue->capacity - fcfs_queue->starting_index + fcfs_queue->ending_index + 1;
}
