CC = gcc
DEFAULT_CFLAGS = -Wall -Wextra -std=c11
DEBUG_CFLAGS = -Wall -Wextra -ggdb -std=c11
LDFLAGS =

# If CFLAGS are not specified, use default flags
CFLAGS ?= $(DEFAULT_CFLAGS)

# Define variables for Windows
ifeq ($(OS),Windows_NT)
	# Makes the Windows version of the logger macro program for Method One
	WINDOWS_MACRO_SRCS = tests\nRF-macro\src\log_macro.c src\logger.c 
	WINDOWS_MACRO_OBJS = tests\nRF-macro\src\log_macro.o src\logger.o 
	WINDOWS_MACRO_TARGET = WIN_nrf-generic.exe

	WINDOWS_EVT_SRCS = tests\nRF-event-driven\src\evt-driven.c src\logger.c src\cpu_info.c
	WINDOWS_EVT_OBJS = tests\nRF-event-driven\src\evt-driven.o src\logger.o src\cpu_info.o
	WINDOWS_EVT_TARGET = WIN_nrf-event-driven.exe



	# Create custom rules for compiling certain folders. 
	
	# For the make clean command 
	RM = del /Q
else
	# Makes the Linux version of the logger macro program for Method One 
	LINUX_MACRO_SRCS = tests/nRF-macro/src/log_macro.c src/logger.c
	LINUX_MACRO_OBJS = tests/nRF-macro/src/log_macro.o src/logger.o
	LINUX_MACRO_TARGET = LIN_nrf-generic

	LINUX_EVT_SRCS = tests/nRF-event-driven/src/evt-driven.c src/logger.c src/cpu_info.c
	LINUX_EVT_OBJS = tests/nRF-event-driven/src/evt-driven.o src/logger.o src/cpu_info.o
	LINUX_EVT_TARGET = LIN_nrf-event-driven
	
	# Reserved for methods later in the publication

	# For the make clean command
	RM = rm -f
endif

.PHONY: all clean-win-macro clean-win-event-driven clean-lin-macro clean-lin-event-driven debug

# Default rule
all: windows-macro windows-event linux-macro linux-event

# Windows macro logging build rule
windows-macro: $(WINDOWS_MACRO_TARGET)

$(WINDOWS_MACRO_TARGET): $(WINDOWS_MACRO_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

# Windows event-driven logging build rule
windows-event: $(WINDOWS_EVT_TARGET)

$(WINDOWS_EVT_TARGET): $(WINDOWS_EVT_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

# Linux macro logging build rule
linux-macro: $(LINUX_TARGET)

$(LINUX_MACRO_TARGET): $(LINUX_MACRO_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

# Linux event-driven logging build rule
linux-event: $(LINUX_EVT_TARGET)
	$(CC) $(LDFLAGS) $^ -o $@

# Compilation rule
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule for Windows macro logging method
clean-win-macro:
	$(RM) $(WINDOWS_MACRO_OBJS) $(WINDOWS_MACRO_TARGET)

# Clean rule for Windows event-driven logging method
clean-win-event-driven:
	$(RM) $(WINDOWS_EVT_OBJS) $(WINDOWS_EVT_TARGET)

# Clean rule for Linux macro logging method
clean-lin-macro:
	$(RM) $(LINUX_MACRO_OBJS) $(LINUX_MACRO_TARGET)

# Clean rule for Linux event-driven logging method
clean-lin-event-driven:
	$(RM) $(LINUX_EVT_OBJS) $(LINUX_EVT_TARGET)

# Debug build rule
debug: CFLAGS=$(DEBUG_CFLAGS)
debug: clean all
