# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

# Directories
SRC_DIR = src
INC_DIR = inc

# Files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:.c=.o)
TARGET = my_project

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $(OBJ_FILES)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJ_FILES) $(TARGET)
