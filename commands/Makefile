CC = gcc
FLAGS = -Wall -Wextra
CFLAGS = -O2 -g $(FLAGS)

# List of source files
SOURCES = $(wildcard *.c)

# List of executable files
EXECUTABLES = $(SOURCES:.c=)

all: $(EXECUTABLES)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLES)