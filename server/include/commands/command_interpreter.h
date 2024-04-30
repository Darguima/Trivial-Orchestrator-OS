#ifndef COMMAND_INTERPRETER_H
#define COMMAND_INTERPRETER_H

#define MAX_COMMAND_LENGTH 100

char* askforcommand();
char** command_interpreter(char* command);

#endif