CFLAGS=-Wall -Wextra -pedantic -O2 -Iinclude
LIBS=-lm -lcurses

jogo.o: src/game/main.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

run:
	make clean
	make
	./jogo.o

clean:
	rm jogo.o