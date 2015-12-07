.PHONY=all clean

CC=gcc
CFLAGS=-Wall -pedantic -std=c11

SRC=$(wildcard src/*.c)
OUT=bin/berkeley

all: $(SRC)
	$(CC) $(CFLAGS) $^ -o $(OUT)

clean:
	rm -v $(OUT)
