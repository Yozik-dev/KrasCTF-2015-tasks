.PHONY=all clean

CC=gcc
CFLAGS=-m32 -Wall -pedantic -std=c11 -static -D_GNU_SOURCE

BINFOLDER=bin/

SOURCE=$(wildcard src/*.c)
BINARY=$(SOURCE:.c=.o)
TARGET=$(BINFOLDER)/x87

all: $(SOURCE)
	@mkdir -pv $(BINFOLDER)
	@echo "compiling " $^
	@$(CC) $(CFLAGS) -o $(TARGET) $^

clean:
	@rm -fv $(BINARY)
	@rm -fv $(TARGET)
