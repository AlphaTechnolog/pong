UNAME_S = $(shell uname -s)
MINGW := $(shell expr "$(UNAME_S)" : 'MINGW')

CFLAGS := -std=c11 -O3 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CFLAGS += -Wno-pointer-arith -Wno-unused-parameter -Wno-newline-eof -Wno-unused-parameter -Wno-gnu-statement-expression
CFLAGS += -Wno-gnu-compound-literal-initializer -Wno-gnu-zero-variadic-macro-arguments
CFLAGS += -I extern/include -L extern/lib

LDFLAGS := -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

ifeq ($(UNAME_S), Linux)
	LDFLAGS += -lm
endif

ifeq ($(MINGW), 5)
	LDFLAGS += -lmingw32
endif

LDFLAGS += -I extern/include -L extern/lib

SRC := $(wildcard src/**/*.c) $(wildcard src/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)
OBJ := $(SRC:.c=.o)
BIN := bin

APPNAME := pong

.PHONY: all clean

all: dirs game

dirs:
	mkdir -p $(BIN)

run: all
	$(BIN)/$(APPNAME)

game: $(OBJ)
	$(CC) -o $(BIN)/$(APPNAME) $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)
