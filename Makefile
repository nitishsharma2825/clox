CC=gcc
CFLAGS=-Wall -Wextra -O2

clox: chunk.c memory.c debug.c value.c main.c
	$(CC) $(CFLAGS) -o clox chunk.c memory.c debug.c value.c main.c

clean:
	rm -f clox *.o