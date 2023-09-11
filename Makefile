CC := gcc

UNAME_S = $(shell uname -s)
MINGW := $(shell expr "$(UNAME_S)" : 'MINGW')

CFLAGS := -std=c11 -O3 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CFLAGS += -Wno-pointer-arith -Wno-unused-parameter -Wno-newline-eof -Wno-unused-parameter -Wno-gnu-statement-expression
CFLAGS += -Wno-gnu-compound-literal-initializer -Wno-gnu-zero-variadic-macro-arguments
CFLAGS += -I extern/include -L extern/lib

LDFLAGS := -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
LDFLAGS += -I extern/include -L extern/lib

ifeq ($(UNAME_S), Linux)
	LDFLAGS += -lm
endif

ifeq ($(MINGW), MINGW)
	LDFLAGS += -lmingw32
endif

SRC := $(wildcard src/**/*.c) $(wildcard src/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)
OBJ := $(SRC:.c=.o)
BIN := bin

APPNAME := pong

.PHONY: all clean

all: dirs prepare game

dirs:
	@mkdir -p $(BIN)

cjson_prepare:
	@echo "Preparing -> cjson"
	@rm src/lib/extern/cJSON/test.c

src/lib/extern/cJSON: cjson_prepare

prepareinit:
	git submodule init
	git submodule update

prepare: prepareinit src/lib/extern/cJSON
	@echo "Done preparing."

run: all
	@echo "Run 		$(BIN)/$(APPNAME)"
	@$(BIN)/$(APPNAME)

game: $(OBJ)
	@echo "Bin 		$(BIN)/$(APPNAME)"
	@$(CC) -o $(BIN)/$(APPNAME) $^ $(LDFLAGS)

%.o: %.c
	@echo "CC		$@"
	@$(CC) -o $@ -c $< $(CFLAGS)

clean:
	@echo "Clean"
	@rm -rf $(BIN) $(OBJ)

build: all
	@echo "Bundling build.zip."
	@mkdir -p ./build
	@echo "=> Copying files to the build directory"
	@cp -rvf ./bin/* ./build
	@cp -rvf ./sprites/ ./build
	@cp -rvf ./fonts ./build
	@cp -rvf ./music ./build
	@echo "=> Bundling into a zip"
	@zip -rv build{.zip,}
	@echo "* Done!"
	@rm -rf ./build
