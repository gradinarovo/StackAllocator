# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I./src -I./base -I./cfg -I./demo
LDFLAGS = -lm

# Directories
SRC_DIR = src
BASE_DIR = base
DEMO_DIR = demo
TARGET_DIR = target
OBJ_DIR = $(TARGET_DIR)/obj
BIN_DIR = $(TARGET_DIR)/bin

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
BASE_FILES = $(wildcard $(BASE_DIR)/*.c)
DEMO_FILES = $(wildcard $(DEMO_DIR)/*.c)

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES)) \
            $(patsubst $(BASE_DIR)/%.c,$(OBJ_DIR)/%.o,$(BASE_FILES)) \
            $(patsubst $(DEMO_DIR)/%.c,$(OBJ_DIR)/%.o,$(DEMO_FILES))

# Executable
TARGET = $(BIN_DIR)/stack_allocator_demo

# Default target
all: dirs $(TARGET)

# Create necessary directories
dirs:
	if not exist "$(TARGET_DIR)" mkdir "$(TARGET_DIR)"
	if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
	if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"

# Link object files
$(TARGET): $(OBJ_FILES)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(BASE_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(DEMO_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	if exist "$(TARGET_DIR)" rmdir /s /q "$(TARGET_DIR)"

# Run the test program
run: all
	./$(TARGET)

# Phony targets
.PHONY: all clean run dirs