CC = gcc
FLAGS = -Wall -Wextra
CFLAGS = -O2 -g $(FLAGS)

SOURCES = $(wildcard *.c)

EXECUTABLES = $(SOURCES:.c=)

all: $(EXECUTABLES)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLES)