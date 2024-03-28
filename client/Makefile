# The name of the final executable files
EXECUTABLE = client

# Find all .c files (SOURCES), and also replace the extension to .o (OBJECTS)
SOURCES = $(shell find ./src -type f -name '*.c')
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))

# Storing the extra arguments passed to the make command, to pass it then to, for example, the executable (make dev)
MAKE_ARGS = $(filter-out $@,$(MAKECMDGOALS))

# Compiler Flags ====
CC = gcc

FLAGS = -Wall -Wextra -pedantic -std=gnu17 -Werror=pedantic -Werror=vla -fstack-protector-all -fstrict-aliasing -Wconversion -Wformat-security -Wundef -Wshadow

# Add correct optimization flags to FLAGS
# To activate DEBUG flags call the make file like this: `DEBUG=1 make ...`
DEBUG ?= 0
ifeq ($(DEBUG), 1)
# Low optimization/fast compilation at debugging/development compilation
	FLAGS += -O0 -g
else
# Lot of optimization/slow compilation at final compilation
	FLAGS += -O3 -Werror
endif

CFLAGS = ${FLAGS} -I include/ -lm

# Commands ===

# Compile projects
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

clean:
	@find . -type f -name '*.o' -delete && echo "[Cleaning] Object Files"
	@rm -f ${EXECUTABLE} && rm -f ${TEST_EXECUTABLE} && echo "[Cleaning] ${EXECUTABLE} and ${TEST_EXECUTABLE}"
	@rm -rf Resultados/command*_output.txt Resultados/*_errors.csv Resultados/tests_results.txt; echo "[Cleaning] Resultados Folder"

format:
# Check if clang is installed
	@command -v clang-format &> /dev/null || echo "[Warning] Please install `clang`. Read documentation."
# Run clang-format against the code files
	@find . -name '*.[ch]' -exec clang-format -i {} -i -style=file \; && echo "[Formating]"

check-memory:
# Check if valgrind is installed
	@command -v valgrind &> /dev/null || echo "[Warning] Please install `valgrind`. Read documentation."
# Run valgrind against the executables
	@DEBUG=1 make -s && echo "[Compiling]"
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./$(EXECUTABLE) $(if $(MAKE_ARGS),$(MAKE_ARGS),"#default args") 

dev:
	@printf "=====  Cleaning, Formating, Compiling and executing projects  =====\n"
	@make -s clean
	@make -s format
	@DEBUG=1 make -s && echo "[Compiling]"
	@printf "===================================================================\n"

	@./$(EXECUTABLE) $(MAKE_ARGS)

	@printf "===================================================================\n"

# Ignore unmatched commands - to allow storing the arguments inside a variable MAKE_ARGS
%:
	@:
