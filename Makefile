CC=gcc
CFLAGS=-I.

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

run: src/main.c
	@echo "building"
	$(CC) -o build/main src/main.c
	./build/main

.PHONY: clean
clean:
	rm -rf build/*
