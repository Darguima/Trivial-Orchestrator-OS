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
#include <signal.h>

#include "../../datapipe/globals.h"
#include "fifo/create.h"
#include "scheduler/scheduler.h"
#include "commands/command_interpreter.h"
#include "commands/command_runner.h"


int active_tasks = 0; // Active tasks counter
Process processes[MAX_SIMULTANEOUS]; // Array to store the processes in execution
int process_count = 0; // Number of processes (fifos) in execution



void execute_process(Process process) {
    int pid;
    pid = fork();
    if (pid == -1) {
        perror("Failed to fork");
        return;
    }
    if (pid == 0) { // Child process    
        if (is_a_simple_process(process->command)) {
            execute_simple_process(process);
        }
        else {
            execute_pipeline_process(process);
        }
        _exit(EXIT_SUCCESS);
     
        
}
 else {
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
                char* buffer = malloc(sizeof(char) * MAX_BUF_SIZE);
                
                char* command =  get_command(processes[i]->command);           
     
                 sprintf(buffer, "%d %s %.4fs\n", processes[i]->id,command ,duration);
                 free(command);
                //writing to log file 
                int fd = open(LOG_PATH, O_WRONLY | O_CREAT | O_APPEND, 0666);
                if (fd == -1) {
                    perror("Failed to open log file");
                    exit(EXIT_FAILURE);
                }
                int check = write(fd, buffer, strlen(buffer));
                if (check == -1) {
                    perror("Failed to write to log file");
                    exit(EXIT_FAILURE);
                }
                close(fd);
                free(buffer);
                remove_processes(i);
                break;
            }
        }
    }
}

void signal_handler(int signum) {
    if (signum == SIGINT || signum == SIGTERM) {
        for (int i = 0; i < active_tasks; i++) { // kill all the processes before exiting
            kill(processes[i]->process_id, SIGKILL);
        }
        delete_fifo(S_FIFO_PATH); // Delete the server FIFO
        exit(EXIT_SUCCESS);
    }
}

int main() {
    signal(SIGINT, signal_handler); // CTRL + C
    signal(SIGTERM, signal_handler); // exit
    Scheduler scheduler = create_scheduler(FCFS);
    char* buffer = malloc(sizeof(char) * MAX_BUF_SIZE);
    char* response = malloc(sizeof(char) * MAX_BUF_SIZE);

    create_fifo(S_FIFO_PATH); // Create the server FIFO

    int server_fd = open(S_FIFO_PATH, O_RDONLY);
    if (server_fd == -1) {
        perror("Error opening server FIFO");
        exit(EXIT_FAILURE);
    }

    while (1) {
        handle_finished_task(); // Verify if any task has finished
         memset(buffer, 0, MAX_BUF_SIZE);
         memset(response, 0, MAX_BUF_SIZE);
        if (read(server_fd, buffer, MAX_BUF_SIZE) > 0) {
            char** command_args = command_interpreter(buffer);
            char* client_id = command_args[0];
            char* client_fifo_path = malloc(sizeof(char) * MAX_FIFO_NAME);
            sprintf(client_fifo_path, "%s_%s", C_FIFO_PATH, client_id);

            
            if (strcmp(command_args[1],"status") == 0) {
            status_writer(client_fifo_path,processes, active_tasks);
            free(command_args);
            free(client_fifo_path);
            memset(buffer, 0, MAX_BUF_SIZE);
            memset(response, 0, MAX_BUF_SIZE);
            continue;
            }

    
            // advance 1 blank space in the buffer
           else { char* command = strchr(buffer, ' ') + 1;
            char* new_command = strdup (command);  
                int id = enqueue_process(scheduler,new_command, atoi(command_args[2])); // Enqueue the process

                // Send the response to the client
                int client_fifo_fd = open(client_fifo_path, O_WRONLY);
                if (client_fifo_fd == -1) {
                    perror("Error opening client FIFO");
                    exit(EXIT_FAILURE);
                }
                sprintf(response, "Task ID = %d\n", id);

                int check = write(client_fifo_fd, response, strlen(response));
                if (check == -1) {
                    perror("Failed to write to client FIFO");
                }
                close(client_fifo_fd);

        }

           
            
           
            free(command_args);
            free(client_fifo_path);
            memset(buffer, 0, MAX_BUF_SIZE);
            memset(response, 0, MAX_BUF_SIZE);

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
    free(buffer);
    free(response);

    return 0;
}