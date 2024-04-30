#include "scheduler/scheduler.h"
#include "stdio.h"
#include "stdlib.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#define SERVER_FIFO "../tmp/fifo_server"

int active_tasks = 0; // Active tasks counter

void execute_process(Process process) {
  printf("Executing process %d\n", process->id);
    int pid, fd;
    pid = fork();
    if (pid == -1) {
        perror("Failed to fork");
        return;
    }
    if (pid == 0) { // Child process
        char log_file[256];
        sprintf(log_file, "../tmp/log");
        fd = open(log_file, O_WRONLY | O_CREAT | O_APPEND, 0666);
        if (fd == -1) {
            perror("Failed to open log file");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        close(fd);

        execlp("./arroz", "arroz", NULL);
        perror("Failed to execute './arroz'");
        exit(EXIT_FAILURE);
    }
    // No redirection in the parent process
}


void handle_finished_task() {
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        active_tasks--;
    }
}


int main() {
  Scheduler scheduler = create_scheduler(FCFS);
 int server_fd, client_fd;
char buffer[1024], client_fifo[256], response[256];



  server_fd = open(SERVER_FIFO, O_RDONLY);
  if (server_fd == -1) {
    perror("Error opening server FIFO");
    exit(EXIT_FAILURE);
  }



   while (1) {
    handle_finished_task(); // Verify if any task has finished

    if (read(server_fd, buffer, sizeof(buffer)) > 0) {
      sscanf(buffer, "%s", client_fifo);

      int id = enqueue_process(scheduler, "ls -l", 10);
      printf("Process enqueued with ID %d\n", id);

      sprintf(response, "%d", id);
       client_fd = open(client_fifo, O_WRONLY);
      write(client_fd, response, strlen(response) + 1);
      close(client_fd);
    }

      if (active_tasks == 0) {
        Process process = dequeue_process(scheduler);
        if (process != NULL ) {
          execute_process(process);
          printf("Process %d started\n", process->id);
          active_tasks = 1; // Increment the number of active tasks
        }
      }
     
   

  }

  destroy_scheduler(scheduler);
  close(server_fd);

  return 0;
}