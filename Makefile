
CC = gcc

CFLAGS = -std=c11 -g -Wall
CFLAGS += -Isrc/server -Isrc/util
LDFLAGS = -g
OUT=mtwwd


SRC = $(wildcard src/*.c) $(wildcard src/**/*.c)
OBJ = $(SRC:.c=.o)

SRC_TEST = $(filter-out src/main.c, $(SRC)) 
SRC_TEST += $(wildcard tests/*.c)
OBJ_TEST = $(SRC_TEST:.c=.o)

BIN = bin

.PHONY: all clean

all: dirs compile

dirs: 
	mkdir -p ./$(BIN)

run: all
	$(BIN)/$(OUT)

test:	compile-test
	$(BIN)/test

compile: $(OBJ)
	$(CC) -o $(BIN)/$(OUT) $^ $(LDFLAGS)

compile-test: $(OBJ_TEST)
	$(CC) -o $(BIN)/test $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)
