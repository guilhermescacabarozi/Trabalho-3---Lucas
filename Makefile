# Makefile para compilar main.c e Lista.c
CC       := gcc
CFLAGS   := -std=c99 -Wall -Wextra -Wpedantic
TARGET   := main
OBJS     := main.o Lista.o

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

# main.c e Lista.c dependem de Lista.h
%.o: %.c Lista.h
	$(CC) $(CFLAGS) -c $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
