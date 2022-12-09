CFLAGS = -O3 -Wall
CC = gcc
OBJS = fork_vs_vfork.o
SRCS = fork_vs_vfork.c
.PHONY: test clean

fork_vs_vfork.out: $(OBJS)
	$(CC) -o fork_vs_vfork.out $(CFLAGS) $(OBJS)

fork_vs_vfork_.o: $(SRCS)
	$(CC) -c $(CFLAGS) $(SRCS)

test: fork_vs_vfork.out
	@echo "test fork 1GB"
	./fork_vs_vfork.out 1 0
	@echo "test vfork 1GB"
	./fork_vs_vfork.out 1 1

clean:
	@rm -f $(OBJS)
	@rm -f fork_vs_vfork.out
