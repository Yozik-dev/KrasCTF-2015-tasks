.PHONY=all compress task clean

DEBUG_COMPILE_FLAG=

CC=gcc
CFLAGS=-Wall -m32 -pedantic -std=c11 $(DEBUG_COMPILE_FLAG)

LEMPEL_ZIV_SRC=$(wildcard src/*.c)
LEMPEL_ZIV_OBJ=$(LEMPEL_ZIV_SRC:.c=.o)

INC=src/include/

BIN_FOLDER=bin
LEMPEL_ZIV_BIN=$(BIN_FOLDER)/lempel_ziv

FORMAT_ALIGN="%-20s"

all: compress

compress: $(LEMPEL_ZIV_SRC)
	@mkdir -v -p $(BIN_FOLDER)
	@printf "compiling \t $(FORMAT_ALIGN)\n" $^
	@$(CC) $(CFLAGS) -o $(LEMPEL_ZIV_BIN) $^ -I$(INC)

task: compress
	@strip $(LEMPEL_ZIV_BIN)
	@upx --all-methods -9 $(LEMPEL_ZIV_BIN)

clean:
	@printf "removing \t $(FORMAT_ALIGN)\n" $(LEMPEL_ZIV_OBJ)
	@rm -f $(LEMPEL_ZIV_OBJ)
	@printf "removing \t $(FORMAT_ALIGN)\n" $(LEMPEL_ZIV_BIN)
	@rm -f $(LEMPEL_ZIV_BIN)

