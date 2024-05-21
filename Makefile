CC = gcc
DEFAULT_CFLAGS = -Wall -Wextra -std=c11
DEBUG_CFLAGS = -Wall -Wextra -ggdb -std=c11
LDFLAGS =

# If CFLAGS are not specified, use default flags
CFLAGS ?= $(DEFAULT_CFLAGS)

ifeq ($(OS),Windows_NT)
    SRCS = tests\nRF-generic\src\log_macro.c src\logger.c
    OBJS = tests\nRF-generic\src\log_macro.o src\logger.o
    TARGET = WIN_nrf-generic.exe
    RM = del /Q
else
    SRCS = tests/nRF-generic/src/log_macro.c src/logger.c
    OBJS = tests/nRF-generic/src/log_macro.o src/logger.o
    TARGET = LIN_nrf-generic
    RM = rm -f
endif

.PHONY: all clean debug

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(TARGET)

debug: CFLAGS=$(DEBUG_CFLAGS)
debug: clean all
