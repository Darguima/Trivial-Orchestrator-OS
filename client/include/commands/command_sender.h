#ifndef COMMAND_SENDER_H
#define COMMAND_SENDER_H

#define C_FIFO_PATH "datapipe/c_fifo"
#define S_FIFO_PATH "datapipe/s_fifo"

int askforcommand();
int send_command(char* command);

#endif