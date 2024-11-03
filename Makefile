CC=gcc
CFLAGS=-I.
DEPS = util.h

%.o: %.c  $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

run: src/main.c src/util.c
	@echo "building"
	$(CC) -o build/main src/main.c src/util.c
	./build/main

.PHONY: clean
clean:
	rm -rf build/*
