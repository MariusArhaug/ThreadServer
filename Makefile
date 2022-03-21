
CC = gcc

CFLAGS = -std=c11 -g -Wall
CFLAGS += -Isrc/ -Ilib/curl/include
CFLAGS += -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700
LDFLAGS = -g -ldl -lpthread

OUT=mtwwd

ENV = .env

WEB_PATH = $(shell grep "WEB_PATH" $(ENV) | cut -d '=' -f2)
PORT = $(shell grep "PORT" $(ENV) | cut -d '=' -f2)
N_THREADS = $(shell grep "N_THREADS" $(ENV) | cut -d '=' -f2)
N_BUFFS = $(shell grep "N_BUFFS" $(ENV) | cut -d '=' -f2)

SRC = $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c)
OBJ = $(SRC:.c=.o)

SRC_TEST = $(filter-out src/main.c, $(SRC)) 
SRC_TEST += $(wildcard tests/*.c)
OBJ_TEST = $(SRC_TEST:.c=.o)

BIN = bin

.POSIX: all
.PHONY: all clean

all: dirs compile

libs:
	cd lib/curl && cmake . && make

dirs: 
	mkdir -p ./$(BIN)

#force run - update port if used
runf:
	$(MAKE) run || $(MAKE) increase_port  

run: all
	$(BIN)/$(OUT) $(WEB_PATH) $(PORT) $(N_THREADS) $(N_BUFFS)

increase_port:
	- $(eval PORT=$(shell echo $$(($(PORT)+$$($$RANDOM % 10)))))
	- $(MAKE) run PORT=$(PORT)

valgrind: all
	valgrind\
	--track-origins=yes\
	--leak-check=full\
	--leak-resolution=high\
	--show-leak-kinds=all $(MAKE) run

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
