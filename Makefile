CC = gcc

CFLAGS = -Wall -Wextra -std=gnu11 -Iinclude

SRC = src/main.c \
      src/ethernet.c \
      src/ipv4.c \
      src/protocol.c \
      src/tcp.c \
      src/udp.c \
      src/icmp.c \
      src/dns.c

OBJ = $(SRC:.c=.o)

TARGET = packet-analyzer

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)