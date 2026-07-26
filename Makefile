CC      := gcc
CFLAGS  := -Wall -Wextra -g -I.
LDFLAGS :=

TARGET  := sniffer

# Recursively find every .c file in the project
SRCS := $(shell find . -name '*.c')
OBJS := $(SRCS:.c=.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# -MMD/-MP auto-generate header dependencies so editing a .h
# triggers a rebuild of everything that includes it
%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

run: $(TARGET)
	sudo ./$(TARGET)

clean:
	find . -name '*.o' -delete
	find . -name '*.d' -delete
	rm -f $(TARGET)