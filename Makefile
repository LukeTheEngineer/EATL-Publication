CC = gcc
CFLAGS = -Wall -Wextra -ggdb -std=c11

SRCDIR = ../../../
SRCS = tests/nRF-generic/src/log_macro.c src/logger.c
OBJS = $(SRCS:.c=.o)

TARGET = nrf-generic

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(TARGET)