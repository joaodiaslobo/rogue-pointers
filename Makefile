CFLAGS=-Wall -Wextra -pedantic -O2 -Iinclude
LIBS=-lm -lcurses -lSDL2 -pthread

TARGET=jogo

SRC = src

SDL := $(shell dpkg -s libsdl2-dev && echo "SDL installed" || sudo apt-get -y install libsdl2-dev && echo "SDL installed") 

SOURCES = $(wildcard src/engine/render/*.c) $(wildcard src/engine/ui/*.c) $(wildcard src/engine/utils/*.c) $(wildcard src/game/*.c) $(wildcard src/game/debug/*.c) $(wildcard src/game/inventory/*.c) $(wildcard src/game/map/*.c) $(wildcard src/game/player/*.c) $(wildcard src/game/ui/*.c) $(wildcard src/game/ai/*.c)
OBJECTS = $(patsubst $(SRC)/%.c, $(SRC)/%.o, $(SOURCES))

all: compile clean

compile: $(OBJECTS)
	-rm -f $(TARGET)
	$(CC) -o $(TARGET) $^ $(LIBS)

$(SRC)/%.o: $(SRC)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

run:
	make compile
	./$(TARGET)

clean:
	-rm -f $(OBJECTS)

.PHONY :  all compile clean run
