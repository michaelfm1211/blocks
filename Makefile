SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

CFLAGS := -Wall -Wextra

all: CFLAGS += -O2 -c
all: blocks docs

.PHONY: debug
debug: CFLAGS += -O0 -g -fsanitize=address
debug:
	$(CC) $(CFLAGS) -o blocks $(SRCS)

.PHONY: docs
docs: blocks.1

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

blocks: $(OBJS)
	$(CC) -o blocks $^

blocks.1: blocks.1.scd
	scdoc < $^ > $@

.PHONY: clean
clean:
	rm -rf $(OBJS) blocks.1 blocks

