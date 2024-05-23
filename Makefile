CC = gcc
DEFAULT_CFLAGS = -Wall -Wextra -std=c11
DEBUG_CFLAGS = -Wall -Wextra -ggdb -std=c11
LDFLAGS =

# If CFLAGS are not specified, use default flags
CFLAGS ?= $(DEFAULT_CFLAGS)

# Define variables for Windows
ifeq ($(OS),Windows_NT)
    WINDOWS_SRCS = tests\nRF-macro\src\log_macro.c src\logger.c
    WINDOWS_OBJS = tests\nRF-macro\src\log_macro.o src\logger.o
    WINDOWS_TARGET = WIN_nrf-generic.exe
    RM = del /Q
else
    # Define variables for Linux
    LINUX_SRCS = tests/nRF-macro/src/log_macro.c src/logger.c
    LINUX_OBJS = tests/nRF-macro/src/log_macro.o src/logger.o
    LINUX_TARGET = LIN_nrf-generic
    RM = rm -f
endif

.PHONY: all clean debug Windows Linux

# Default rule
all: Windows Linux

# Windows build rule
Windows: $(WINDOWS_TARGET)

$(WINDOWS_TARGET): $(WINDOWS_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

# Linux build rule
Linux: $(LINUX_TARGET)

$(LINUX_TARGET): $(LINUX_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

# Compilation rule
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	$(RM) $(WINDOWS_OBJS) $(WINDOWS_TARGET) $(LINUX_OBJS) $(LINUX_TARGET)

# Debug build rule
debug: CFLAGS=$(DEBUG_CFLAGS)
debug: clean all
