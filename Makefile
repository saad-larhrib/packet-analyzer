CC = gcc

CFLAGS = -Wall -Wextra -std=gnu11 -Iinclude

SRC = src/main.c \
      src/ethernet.c \
      src/ipv4.c \
      src/protocol.c \
      src/tcp.c

TARGET = packet-analyzer

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)