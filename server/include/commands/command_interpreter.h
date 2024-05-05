#ifndef COMMAND_INTERPRETER_H
#define COMMAND_INTERPRETER_H

char** command_interpreter(char* command);
void status_writer(char* client_fifo_path, Process processes[MAX_SIMULTANEOUS], int active_tasks); 
char** parse_command(const char* command);
char* get_command (char* command);

#endif