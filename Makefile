CFLAGS=-Wall -Wextra -pedantic -O2
LIBS=-lm -lcurses

jogo.o: src/main.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

run:
	make
	./jogo.o

clean:
	rm jogo *.o