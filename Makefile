CC = gcc
CFLAGS = -Wall -Wextra -ggdb -std=c11
LDFLAGS =

ifeq ($(OS),Windows_NT)
    SRCS = tests\nRF-generic\src\log_macro.c src\logger.c
    OBJS = tests\nRF-generic\src\log_macro.o src\logger.o
    TARGET = WIN_nrf-generic
    RM = del /Q
else
    SRCS = tests/nRF-generic/src/log_macro.c src/logger.c
    OBJS = tests/nRF-generic/src/log_macro.o src/logger.o
    TARGET = LIN_nrf-generic
    RM = rm -f
endif

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(TARGET)
