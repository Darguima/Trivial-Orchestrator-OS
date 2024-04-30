#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands/command_sender.h"

int main(int argc, char** argv) {
  if (argc <= 1) {
    askforcommand();
  } else {
    // command is argv except the first element
    char* command = argv[1];
    send_command(command);
  }
  return 0;
}