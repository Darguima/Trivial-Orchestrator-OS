#ifndef COMMAND_INTERPRETER_H
#define COMMAND_INTERPRETER_H

char **command_interpreter(char *command);
void status_writer(char *client_fifo_path, Process processes[MAX_SIMULTANEOUS], int active_tasks);
char **parse_command(const char *command);
char *get_command(char *command);
char *get_command_pipeline(char *command);
int is_a_simple_process(char *command);
char **parse_command_segment(char *segment);
void execute_pipeline_process(Process process);
void execute_simple_process(Process process);
#endif