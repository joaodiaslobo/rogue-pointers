#include <ncurses.h>
#include <stdlib.h>
#include "map/map.c"
#include "time.h"
#include "game_types.h"
#include "engine_types.h"

// TODO: Este ficheiro tem coisas a mais, algumas funcionalidades devem ser separadas para uma pasta à parte

typedef struct state {
	int playerX;
	int playerY;
} STATE;

/**
 *
 * Um pequeno exemplo que mostra o que se pode fazer
 */
void do_movement_action(STATE *st, int dx, int dy, MAP (*m)[NUM_COLUMNS], int r, int c) {
    int x = st->playerX + dx;
	int y = st->playerY + dy;
    
	//attron(COLOR_PAIR(COLOR_BLUE));
	move(r-1,20);
	printw("col:%d lin:%d tc:%d tl:%d Obs:%d col:%d, lin:%d", st->playerX, st->playerY, c, r, m[y][x].object, x,y);
	//getch();
	//attroff(COLOR_PAIR(COLOR_BLUE));
	 	
	if (m[y][x].object == 1 || m[y][x].object == 3 || x < 0 || y < 0 || y >= r || x >= c){ 
		// valida a posição do jogador no mapa, se este move para uma parede não pode avançar
        // valida que o jogador não sai do mapa
	} else if (m[y][x].object == 2) { //encontrou um porta, muda de nível e gera novo mapa
        gen_map(m,r,c,2);
	    print_map(m,r,c);   
	} 
      else {
          st->playerX += dx;
	      st->playerY += dy;
   	  }
}

void update(STATE *st, MAP (*m)[NUM_COLUMNS], int r, int c) {
	int key = getch();
    
	//mvaddch(st->playerX, st->playerY, ' ');
	switch(key) {
		case KEY_A1:
		case '7': do_movement_action(st, -1, -1, m, r, c); break;
		case KEY_UP:
		case '8': do_movement_action(st, +0, -1, m, r, c); break;
		case KEY_A3:
		case '9': do_movement_action(st, +1, -1, m, r, c); break;
		case KEY_LEFT:
		case '4': do_movement_action(st, -1, +0, m, r, c); break;
		case KEY_B2:
		case '5': break;
		case KEY_RIGHT:
		case '6': do_movement_action(st, +1, +0, m, r, c); break;
		case KEY_C1:
		case '1': do_movement_action(st, -1, +1, m, r, c); break;
		case KEY_DOWN:
		case '2': do_movement_action(st, +0, +1, m, r, c); break;
		case KEY_C3:
		case '3': do_movement_action(st, +1, +1, m, r, c); break;
		case 'q': endwin(); exit(0); break;
	}
}


int game_loop(Terminal *terminal) {
	STATE st = {20,20};
	int ncols, nrows;
    ncols = terminal->xMax;
	ncols = ncols/2;
    nrows = terminal->yMax;
	
	// Criação e inicialização do mapa 
	NUM_COLUMNS = ncols;
	
	MAP (*mp)[ncols] = malloc(sizeof(MAP[nrows][ncols]));
    if (mp == NULL) {
        fprintf(stderr, "Erro: nao foi possivel alocar memoria\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nrows; i++){
      for (int j = 0; j < ncols; j++){
         mp[i][j].object = 3;
      }
    }

	srandom(time(NULL));

	intrflush(stdscr, false);
	keypad(stdscr, true);

	init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
	init_pair(COLOR_RED, COLOR_MAGENTA, COLOR_BLACK);
    
    // Gera e imprime o mapa
    gen_map(mp,nrows,ncols,2);
	print_map(mp,nrows,ncols);
	getch();
	
	endwin(); 

	
	/**
	 * Este código está muito mal escrito!
	 * Deveria existir uma função chamada draw_player!
	 *
	 * Se estamos a desenhar uma luz à volta do jogador
	 * deveria existir uma função chamada draw_light!
	 *
	 */

	Image characterSprite = load_image_from_file("assets/sprites/characters/player1.sprite");

	
	while(1) {
		clear();
		move(nrows - 1, 0);
		attron(COLOR_PAIR(COLOR_BLUE));
		printw("(%d, %d) %d %d", st.playerX, st.playerY, ncols, nrows);
		attroff(COLOR_PAIR(COLOR_BLUE));
		//attron(COLOR_PAIR(COLOR_BLACK));
		//attroff(COLOR_PAIR(COLOR_RED));
		print_map(mp,nrows,ncols);
		/*Image gate = load_image_from_file("assets/sprites/gate.sprite"); Não apagar estas 3 linhas, usadas p/ testes
	    Vector2D pos = {st.playerX,st.playerY};
	    draw_to_screen(gate, pos); */
		Vector2D pos = {st.playerX, st.playerY}; 
		draw_to_screen(characterSprite, pos);
		/*
		Se utilizarmos apenas 1 quadrado como player, as funcionalidades de não atravessar paredes e descer de níveis, 
		já funcionam. Não consegui colocar o Dan a funcionar no contexto, pois ele não possui um centro de massa, ou seja,
		um ponto sincronizado com o mapa. Além disso, com a forma atual do Dan, na horizonal ele só percorre caminhos de 6
		e na vertical caminhos de 4, ou seja, não sei se faz propriamente sentido e, por isso, apenas tratei desta funciona-
		lidade para um ponto.
		*/
		//attron(COLOR_PAIR(COLOR_BLUE));
		mvaddch(st.playerX - 1, st.playerY - 1, '.' | A_BOLD);
		mvaddch(st.playerX - 1, st.playerY + 0, '.' | A_BOLD);
		mvaddch(st.playerX - 1, st.playerY + 1, '.' | A_BOLD);
		mvaddch(st.playerX + 0, st.playerY - 1, '.' | A_BOLD);
		mvaddch(st.playerX + 0, st.playerY + 1, '.' | A_BOLD);
		mvaddch(st.playerX + 1, st.playerY - 1, '.' | A_BOLD);
		mvaddch(st.playerX + 1, st.playerY + 0, '.' | A_BOLD);
		mvaddch(st.playerX + 1, st.playerY + 1, '.' | A_BOLD);
        //attroff(COLOR_PAIR(COLOR_YELLOW));
		//move(st.playerX, st.playerY);
		update(&st,mp,nrows,ncols);
	} 
	return 0;
}