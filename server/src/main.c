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

void status_write_executing (char* client_fifo_path) {

    char* buffer = malloc(sizeof(char) * MAX_BUF_SIZE);
    if (buffer == NULL) {
        perror("Failed to allocate memory for buffer");
        exit(EXIT_FAILURE);
    }

    int client_fifo_fd = open(client_fifo_path, O_WRONLY);
    if (client_fifo_fd == -1) {
        perror("Error opening client FIFO");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    // Send the header
    snprintf(buffer, MAX_BUF_SIZE, "Executing\n");
    write(client_fifo_fd, buffer, strlen(buffer));
    //memset(buffer, 0, MAX_BUF_SIZE);

    // Send the processes in execution
    for (int i = 0; i < active_tasks; i++) {
        snprintf(buffer, MAX_BUF_SIZE, "Task ID = %d %s\n", processes[i]->id, processes[i]->command);
        write(client_fifo_fd, buffer, strlen(buffer));
        memset(buffer, 0, MAX_BUF_SIZE);
    }
    free(buffer);

     char* buffer_2 = malloc(sizeof(char) * MAX_BUF_SIZE);
    if (buffer_2 == NULL) {
        perror("Failed to allocate memory for buffer");
        exit(EXIT_FAILURE);
    }


    snprintf(buffer_2, MAX_BUF_SIZE, "Finished\n");
    write(client_fifo_fd, buffer_2, strlen(buffer_2));

    int log_fd = open(LOG_PATH, O_RDONLY);
    if (log_fd == -1) {
        perror("Error opening log file");
        free(buffer_2);
        exit(EXIT_FAILURE);
    }

    while (read(log_fd, buffer_2, MAX_BUF_SIZE) > 0) {
        write(client_fifo_fd, buffer_2, strlen(buffer_2));
        memset(buffer_2, 0, MAX_BUF_SIZE);
    }

    close(log_fd);
    close(client_fifo_fd);
    free(buffer_2);

}

void status_write_finished (char* client_fifo_path) {
  // opens log file and sends it to the client with the header: Finished
    char* buffer = malloc(sizeof(char) * MAX_BUF_SIZE);
    if (buffer == NULL) {
        perror("Failed to allocate memory for buffer");
        exit(EXIT_FAILURE);
    }

    int client_fifo_fd = open(client_fifo_path, O_WRONLY);
    if (client_fifo_fd == -1) {
        perror("Error opening client FIFO");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    snprintf(buffer, MAX_BUF_SIZE, "Finished\n");
    write(client_fifo_fd, buffer, strlen(buffer));

    int log_fd = open(LOG_PATH, O_RDONLY);
    if (log_fd == -1) {
        perror("Error opening log file");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    while (read(log_fd, buffer, MAX_BUF_SIZE) > 0) {
        write(client_fifo_fd, buffer, strlen(buffer));
        memset(buffer, 0, MAX_BUF_SIZE);
    }

    close(log_fd);
    close(client_fifo_fd);
    free(buffer);
  
}

void status_writer(char* client_fifo_path) {
    if (fork () == 0 ) {
    status_write_executing(client_fifo_path);
   // status_write_finished(client_fifo_path);
      _exit(0);
    }
   
  

}


char** parse_command(const char* command) {
    printf ("[DEBUG] - Parsing command: %s\n", command);
    // Creates a copy of the command string
    char* buffer = strdup(command);
    if (buffer == NULL) {
        printf("Failed to duplicate command string");
    }

    int arg_count = 0;
    char** args = NULL;
    char* token;
    const char* delim = " ";

    
    strtok(buffer, delim); // "execute"
    strtok(NULL, delim);   // "time"
    strtok(NULL, delim);   // "-u"

    // Process the remaining arguments
    while ((token = strtok(NULL, delim)) != NULL) {
        // Realocates the array of arguments
        char** new_args = realloc(args, (arg_count + 2) * sizeof(char*)); // +2 para novo arg e NULL
        if (new_args == NULL) {
           printf("Failed to reallocate arguments array");
            free(args); // Free the old array
            free(buffer); // Free the buffer
            return NULL;
        }
        args = new_args;
        args[arg_count++] = strdup(token);
    }
    if (args != NULL) {
        args[arg_count] = NULL;  // Last argument is NULL
    }

    free(buffer); // Free the buffer
    return args;
}



void execute_process(Process process) {
    int pid;
    pid = fork();
    if (pid == -1) {
        perror("Failed to fork");
        return;
    }
    if (pid == 0) { // Child process
     
        char** args = parse_command(process->command); // this memory will be freed by the execvp function
        char file_process[256]; // format is process_id.txt
        sprintf(file_process, "%s_%d.txt",TASKS_PATH,process->id);
        int fd = open(file_process, O_WRONLY | O_CREAT | O_APPEND, 0666);
        if (fd == -1) {
            perror("Failed to open log file");
            exit(EXIT_FAILURE);
        }

        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        close(fd);

        int check = execvp(args[0], args);

       if (check == -1) {
            perror("Failed to execute command");
            exit(EXIT_FAILURE);
        }

        
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
                sprintf(buffer, "%d %s %.4fs\n", processes[i]->id,processes[i]->command ,duration);
                //writing to log file 
                int fd = open(LOG_PATH, O_WRONLY | O_CREAT | O_APPEND, 0666);
                if (fd == -1) {
                    perror("Failed to open log file");
                    exit(EXIT_FAILURE);
                }
                write(fd, buffer, strlen(buffer));
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
            status_writer(client_fifo_path);
            free(command_args);
            free(client_fifo_path);
            memset(buffer, 0, MAX_BUF_SIZE);
            memset(response, 0, MAX_BUF_SIZE);
            continue;
            }

    
            // advance 1 blank space in the buffer
           else { char* command = strchr(buffer, ' ') + 1;
            char* new_command = strdup (command);
                int id = enqueue_process(scheduler,new_command, 5); // Enqueue the process

                // Send the response to the client
                int client_fifo_fd = open(client_fifo_path, O_WRONLY);
                if (client_fifo_fd == -1) {
                    perror("Error opening client FIFO");
                    exit(EXIT_FAILURE);
                }
                sprintf(response, "Task ID = %d\n", id);

                write(client_fifo_fd, response, strlen(response));
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