CC=gcc
CFLAGS=-Wall -Wextra -O0 -g

clox: chunk.c memory.c object.c debug.c value.c vm.c scanner.c compiler.c table.c main.c
	$(CC) $(CFLAGS) -o clox chunk.c memory.c object.c debug.c value.c vm.c scanner.c compiler.c table.c main.c

clean:
	rm -f clox *.o