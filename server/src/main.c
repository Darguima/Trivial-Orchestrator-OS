#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../../datapipe/globals.h"
#include "fifo/create.h"
#include "scheduler/scheduler.h"
#include "commands/command_interpreter.h"
#include "commands/command_runner.h"

int active_tasks = 0; // Active tasks counter
Process processes[MAX_SIMULTANEOUS]; // Array to store the processes in execution
int process_count = 0; // Number of processes (fifos) in execution

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
        sprintf(log_file, LOG_PATH);
        fd = open(log_file, O_WRONLY | O_CREAT | O_APPEND, 0666);
        if (fd == -1) {
            perror("Failed to open log file");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        close(fd);

        execlp("bin/test_command", "test_command", NULL);
        perror("Failed to execute 'bin/test_command'");
        exit(EXIT_FAILURE);
    } else {
        process->process_id = pid;
        if (active_tasks < MAX_SIMULTANEOUS) {
            processes[active_tasks++] = process;
        }
    }
}

void remove_processes(int index) {
    if (index < active_tasks - 1) {
        processes[index] = processes[active_tasks - 1];  
    }
    active_tasks--;
}

void handle_finished_task() {
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        for (int i = 0; i < active_tasks; i++) {
            if (processes[i]->process_id == pid) {
                //calculate duration of the process in seconds
                struct timeval end_time;
                gettimeofday(&end_time, NULL);
                double duration = (double)(end_time.tv_sec - processes[i]->start_time.tv_sec) + (double)(end_time.tv_usec - processes[i]->start_time.tv_usec) / 1000000.0;
                printf("Debug: Process %d with PID %d has finished. Duration: %.6f seconds.\n", processes[i]->id , pid, duration); // just testing the time of the process
                remove_processes(i);
                break;
            }
        }
    }
}

/*TODO: ADD TO COMMAND RUNNER THIS LOGIC
int main() {
    Scheduler scheduler = create_scheduler(FCFS);
    char buffer[MAX_BUF_SIZE], client_fifo[MAX_FIFO_NAME], response[MAX_BUF_SIZE];

    // Create server FIFO
    create_fifo(S_FIFO_PATH);

    int server_fd = open(S_FIFO_PATH, O_RDONLY);
    if (server_fd == -1) {
        perror("Error opening server FIFO");
        exit(EXIT_FAILURE);
    }

    while (1) {
        handle_finished_task(); // Verify if any task has finished

        if (read(server_fd, buffer, sizeof(buffer)) > 0) {
            sscanf(buffer, "%s", client_fifo);

            int id = enqueue_process(scheduler, "ls -l", 10);

            sprintf(response, "%d", id);
            int client_fd = open(client_fifo, O_WRONLY);
            if (write(client_fd, response, strlen(response) + 1) == -1) {
                perror("Failed to write to client FIFO");
            }
            close(client_fd);
        }

        while (active_tasks < MAX_SIMULTANEOUS) {
            Process process = dequeue_process(scheduler);
            if (process != NULL) {
                execute_process(process);
            } else {
                break;  // There are no more processes to execute
            }
        }
    }

    destroy_scheduler(scheduler);
    close(server_fd);

    return 0;
}
}*/

int main() {
    char** fifo_connections = malloc(sizeof(char*) * MAX_SIMULTANEOUS);
    for (int i = 0; i < MAX_SIMULTANEOUS; i++) {
        fifo_connections[i] = malloc(sizeof(char) * MAX_FIFO_NAME);
    }
    Scheduler scheduler = create_scheduler(FCFS);

    // Create server FIFO
    create_fifo(S_FIFO_PATH);

    // Open server FIFO for reading
    int s_fd = open(S_FIFO_PATH, O_RDONLY, 0600);
    if (s_fd == -1) {
        perror("open");
        return 1;
    }

    char* buffer = malloc(sizeof(char) * MAX_BUF_SIZE);
    //ssize_t read_bytes;

    /*while ((read_bytes = read(s_fd, buffer, MAX_BUF_SIZE)) > 0) {
        char** command_args = command_interpreter(buffer);
        char* client_fifo = command_args[1];
        if (strcmp(command_args[0], "create_taskuser") == 0) {
            // create_taskuser <client_fifo>
            if (client_fifo != NULL) {
                create_fifo(client_fifo); // creates the user FIFO
                strcpy(fifo_connections[process_count], client_fifo);
                process_count++;
            }
        } else if (strcmp(command_args[0], "delete_taskuser") == 0) {
            // delete_taskuser <client_fifo>
            if (client_fifo != NULL) {
                delete_fifo(client_fifo); // deletes the user FIFO
            }
        } else {
            // Execute the command
            int id = enqueue_process(scheduler, buffer, 10);
            char response[MAX_BUF_SIZE];
            sprintf(response, "%d", id);
            int client_fd = open(client_fifo, O_WRONLY);
            if (write(client_fd, response, strlen(response) + 1) == -1) {
                perror("Failed to write to client FIFO");
            }
            close(client_fd);
        }
        free(command_args);
        memset(buffer, 0, MAX_BUF_SIZE); // Reset buffer to null characters (clear it)
    }*/

    while (1) {
        handle_finished_task(); // Verify if any task has finished

        if (read(s_fd, buffer, sizeof(buffer)) > 0) {
            char** command_args = command_interpreter(buffer);
            char* client_fifo = command_args[1];
            if (strcmp(command_args[0], "create_taskuser") == 0) {
                // create_taskuser <client_fifo>
                if (client_fifo != NULL) {
                    create_fifo(client_fifo); // creates the user FIFO
                    strcpy(fifo_connections[process_count], client_fifo);
                    process_count++;
                }
            } else if (strcmp(command_args[0], "delete_taskuser") == 0) {
                // delete_taskuser <client_fifo>
                if (client_fifo != NULL) {
                    delete_fifo(client_fifo); // deletes the user FIFO
                }
            } else {
                // Execute the command
                int id = enqueue_process(scheduler, buffer, 10);
                char response[MAX_BUF_SIZE];
                sprintf(response, "%d", id);
                int client_fd = open(client_fifo, O_WRONLY);
                if (write(client_fd, response, strlen(response) + 1) == -1) {
                    perror("Failed to write to client FIFO");
                }
                close(client_fd);
            }
            free(command_args);
            memset(buffer, 0, MAX_BUF_SIZE); // Reset buffer to null characters (clear it)
        }

        while (active_tasks < MAX_SIMULTANEOUS) {
            Process process = dequeue_process(scheduler);
            if (process != NULL) {
                execute_process(process);
            } else {
                break;  // There are no more processes to execute
            }
        }
    }

    // On server close delete the FIFO
    delete_fifo(S_FIFO_PATH);
    for (int i = 0; i < process_count; i++) {
        delete_fifo(fifo_connections[i]);
    }
    for (int i = 0; i < MAX_SIMULTANEOUS; i++) {
        free(fifo_connections[i]);
    }
    free(fifo_connections);
    destroy_scheduler(scheduler);
    close(s_fd);
    free(buffer);
    return 0;
}