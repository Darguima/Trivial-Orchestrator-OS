#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "hello_world/hello_world.h"

#define SERVER_FIFO "../tmp/fifo_server"
#define SERVER_LOG "../tmp/server_log.txt"
#define MAX_TASKS 100

// se tas aqui de paraquedas, vou explicar oq foi feito.
/*
basicamente o servidor recebe qualquer coisa do client (apesar de ser executado sempre o mesmo comando no execlp),
adiciona a tarefa a um array com o seu estado envia para o cliente o numero da tarefa  e executa a tarefa, guardando o
output na pasta tmp no ficheiro server_log.txt. Não é o melhor exemplo de um servidor, mas é um exemplo de um servidor
que executa tarefas e guarda o output num ficheiro. Falta implementar o parser do Fábio e já da para executar coisas
mais especificas.


*/

typedef enum { PENDING, EXECUTING, COMPLETED } EstadoTarefa;

typedef struct {
  int id;
  EstadoTarefa estado;
} Tarefa;

Tarefa tarefas[MAX_TASKS];
int task_count = 0;

void execute_task(int task_id) {
  int fd;
  pid_t pid;

  tarefas[task_id].estado = EXECUTING;

  pid = fork();
  if (pid == -1) {
    perror("Error creating child process");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    fd = open(SERVER_LOG, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd == -1) {
      perror("Error opening log file");
      exit(EXIT_FAILURE);
    }
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    close(fd);

    execlp("ls", "ls", "-l", NULL);
    perror("Error executing command");
    exit(EXIT_FAILURE);
  } else {
    wait(NULL);
    tarefas[task_id].estado = COMPLETED;
  }
}

int main() {
  int server_fd, client_fd;
  char buffer[1024], client_fifo[256], response[256];

  if (mkfifo(SERVER_FIFO, 0666) == -1) {
    perror("Error creating server FIFO");
    exit(EXIT_FAILURE);
  }

  server_fd = open(SERVER_FIFO, O_RDONLY);
  if (server_fd == -1) {
    perror("Error opening server FIFO");
    exit(EXIT_FAILURE);
  }

  printf("Server Starting...\n Hold on for a moment \n Server Started\n Waiting for clients...\n");
  while (1) {
    if (read(server_fd, buffer, sizeof(buffer)) > 0) {
      sscanf(buffer, "%s", client_fifo);

      tarefas[task_count].id = task_count;
      tarefas[task_count].estado = PENDING;
      sprintf(response, "%d", task_count);
      client_fd = open(client_fifo, O_WRONLY);
      ssize_t n = write(client_fd, response, strlen(response) + 1);
      close(client_fd);
      n = n + 1;

      execute_task(task_count);
      task_count++;
    }
  }

  close(server_fd);
  unlink(SERVER_FIFO);

  return EXIT_SUCCESS;
}
