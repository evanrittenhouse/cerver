CC=gcc
CFLAGS=-I.

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

hellomake: src/main.c
	$(CC) -o build/main src/main.c
	./build/main
