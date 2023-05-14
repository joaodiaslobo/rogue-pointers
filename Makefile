CFLAGS=-Wall -Wextra -pedantic -O2 -Iinclude
LIBS=-lm -lcurses -lSDL2

TARGET=game

SRC = src
BIN = bin

SDL := $(shell dpkg -s libsdl2-dev && echo "SDL installed" || sudo apt-get -y install libsdl2-dev && echo "SDL installed") 

SOURCES = $(wildcard src/engine/render/*.c) $(wildcard src/engine/ui/*.c) $(wildcard src/engine/utils/*.c) $(wildcard src/game/*.c) $(wildcard src/game/debug/*.c) $(wildcard src/game/inventory/*.c) $(wildcard src/game/map/*.c) $(wildcard src/game/player/*.c) $(wildcard src/game/ui/*.c) $(wildcard src/game/ai/*.c)
OBJECTS = $(patsubst $(SRC)/%.c, $(SRC)/%.o, $(SOURCES))

all: compile clean

compile: $(OBJECTS)
	-rm $(BIN)/$(TARGET)
	$(CC) -o $(BIN)/$(TARGET) $^ $(LIBS)

$(SRC)/%.o: $(SRC)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

run:
	make compile
	./$(BIN)/$(TARGET)

clean:
	-rm $(OBJECTS)

.PHONY :  all compile clean run
