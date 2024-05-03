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
#include <process.h>

#include "../../datapipe/globals.h"
#include "fifo/create.h"
#include "scheduler/scheduler.h"
#include "commands/command_interpreter.h"
#include "commands/command_runner.h"

int active_tasks = 0;                // Active tasks counter
Process processes[MAX_SIMULTANEOUS]; // Array to store the processes in execution
int process_count = 0;               // Number of processes (fifos) in execution

void execute_process(Process process)
{
    printf("[DEBUG] - Executing process %d\n", process->id);
    int pid;
    pid = fork();
    if (pid == -1)
    {
        perror("Failed to fork");
        return;
    }
    if (pid == 0)
    { // Child process
        /*char log_file[256];
        sprintf(log_file, LOG_PATH);
        int fd = open(log_file, O_WRONLY | O_CREAT | O_APPEND, 0666);
        if (fd == -1) {
            perror("Failed to open log file");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        close(fd);
        execlp("bin/test_command", "test_command", NULL);
        perror("Failed to execute 'bin/test_command'");
        exit(EXIT_FAILURE);*/
        printf("[DEBUG] - Executing command: %s\n", process->command);
    }
    else
    {
        process->process_id = pid;
        if (active_tasks < MAX_SIMULTANEOUS)
        {
            processes[active_tasks++] = process;
        }
    }
}

void remove_processes(int index)
{
    if (index < active_tasks - 1)
    {
        processes[index] = processes[active_tasks - 1];
    }
    active_tasks--;
}

void handle_finished_task()
{
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        for (int i = 0; i < active_tasks; i++)
        {
            if (processes[i]->process_id == pid)
            {
                // calculate duration of the process in seconds
                struct timeval end_time;
                gettimeofday(&end_time, NULL);
                double duration = (double)(end_time.tv_sec - processes[i]->start_time.tv_sec) + (double)(end_time.tv_usec - processes[i]->start_time.tv_usec) / 1000000.0;
                printf("[DEBUG] - Process %d with PID %d has finished. Duration: %.6f seconds.\n", processes[i]->id, pid, duration); // just testing the time of the process
                remove_processes(i);
                break;
            }
        }
    }
}

void signal_handler(int signum)
{
    if (signum == SIGINT || signum == SIGTERM)
    {
        for (int i = 0; i < active_tasks; i++)
        { // kill all the processes before exiting
            kill(processes[i]->process_id, SIGKILL);
        }
        delete_fifo(S_FIFO_PATH); // Delete the server FIFO
        exit(EXIT_SUCCESS);
    }
}

int main()
{
    Scheduler scheduler = create_scheduler(SJF);

    char command[100];
    int process_id;
    Process process;

    for (int i = 13; i >= 0; i--)
    {
        sprintf(command, "ls %d", i);
        process_id = enqueue_process(scheduler, command, i);
        printf("Enqueued Process %d: %s\n", process_id, command);
    }

    for (int i = 0; i < 14; i++)
    {
        process = dequeue_process(scheduler);

        if (process != NULL)
        {
            printf("Dequeued Process %d: %s\n", process->id, process->command);
            free(process->command);
        }

        free(process);
    }

    destroy_scheduler(scheduler);

    return 0;
}