#ifndef COMMAND_SENDER_H
#define COMMAND_SENDER_H

int askforcommand(int client_pid);
int send_command(int client_pid, char* command);
void send_create_task_fifo(int client_pid);
void send_delete_task_fifo(int client_pid);

#endif