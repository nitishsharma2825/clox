CC=gcc
CFLAGS=-Wall -Wextra -O0 -g

clox: chunk.c memory.c debug.c value.c vm.c scanner.c compiler.c main.c
	$(CC) $(CFLAGS) -o clox chunk.c memory.c debug.c value.c vm.c scanner.c compiler.c main.c

clean:
	rm -f clox *.o