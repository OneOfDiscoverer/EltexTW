BIN := main
SOURCES := main.c list.c
HEADERS := main.h list.h
CFLAGS := -g
LDFLAGS := -lrt -pthread

CC := gcc

.PHONY: clean


all: clean $(BIN)

$LD_LIBRARY_PATH=$PWD:$LD_LIBRARY_PATH

$(BIN): $(SOURCES)
		$(CC) $(SOURCES) -o $(BIN) $(CFLAGS) $(LDFLAGS) -O2
$(BIN): $(HEADERS)

test:	
		rm -rf test
		$(CC) test.c list.c -o test -lcmocka
.PHONY: test

clean:
		rm -rf $(BIN)
