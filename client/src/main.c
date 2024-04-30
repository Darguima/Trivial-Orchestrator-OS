#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../datapipe/globals.h"
#include "commands/command_sender.h"

int main(int argc, char** argv) {
  if (argc <= 1) {
    askforcommand();
  } else {
    // command is argv except the first element
    char* command = (char*)malloc(MAX_COMMAND_LENGTH * sizeof(char));
    for (int i = 1; i < argc; i++) {
      strcat(command, argv[i]);
      if (i < argc - 1) {
        strcat(command, " ");
      }
    }
    send_command(command);
    free(command);
  }
  return 0;
}