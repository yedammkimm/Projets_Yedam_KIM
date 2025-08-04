# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Include directories
INCLUDES = -I.

# Source files
SRCS = main_tactics.c tactics.c clientAPI.c snakeAPI.c arenasnake.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = tactics_game

# Default target
all: $(EXEC)

# Link object files to create the executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJS) $(EXEC)

# Run the executable
run: $(EXEC)
	./$(EXEC)
