
CC = gcc

CFLAGS = -std=c11 -g -Wall
CFLAGS += -Isrc/
LDFLAGS = -g

OUT=mtwwd

ENV = .env

WEB_PATH = $(shell grep "WEB_PATH" $(ENV) | cut -d '=' -f2)
PORT = $(shell grep "PORT" $(ENV) | cut -d '=' -f2)
N_THREADS = $(shell grep "N_THREADS" $(ENV) | cut -d '=' -f2)
N_BUFFS = $(shell grep "N_BUFFS" $(ENV) | cut -d '=' -f2)

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
	$(BIN)/$(OUT) $(WEB_PATH) $(PORT) $(N_THREADS) $(N_BUFFS)

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
