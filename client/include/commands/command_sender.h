#ifndef COMMAND_SENDER_H
#define COMMAND_SENDER_H

int askforcommand();
int send_command(char* command);
void send_create_task_fifo(char* client_fifo);
void send_delete_task_fifo(char* client_fifo);

#endif