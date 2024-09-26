# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Werror -Wextra -pedantic -Iinc -g

# Directories
SRCDIR = src
INCDIR = inc
OBJDIR = build
BINDIR = bin

# Source files and object files
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Executable target
TARGET = $(BINDIR)/raycasting_game

# SDL2 libraries
SDL_LIB = -lSDL2 -lSDL2_image -lm

# Default target: Build the project
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(SDL_LIB)

# Build object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Phony targets
.PHONY: all clean
