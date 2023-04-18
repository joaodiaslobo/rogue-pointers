#include <ncurses.h>
#include "engine_types.h"
#include "ui/main_menu.c"
#include "debug/pixel_test.c"
#include "../engine/render/color.c"
#include "map/map.c"



int main(){
    // Setup do ncurses
    WINDOW *window = initscr();

    // Certifica-se que o terminal onde o jogo está a rodar suporta a utilização de cores
    if(!can_change_color()){
        printw("Terminal não suporta cores.");
        getch();
        return -1;
    }

    // Faz com que o cursor fique invisível durante o jogo
    curs_set(0);

    cbreak();
	noecho();

    start_color();

    // Carrega a palette para o ncurses
    if(!load_palette_from_file("assets/main.palette")){
        printw("Falha no carregamento da palette.");
    }

    // Obtem tamanho do terminal
    Terminal terminal;
    getmaxyx(stdscr, terminal.yMax, terminal.xMax);

    int selection = main_menu(&terminal);
    switch (selection)
    {
    case 3:
        pixel_test();
        break;
	case 0:
		mapa_teste(window);
		break;
    default:
        break;
    }

    // Finaliza o ncurses
    endwin();
    return 0;
}

typedef struct state {
	int playerX;
	int playerY;
} STATE;


/**
 *
 * Um pequeno exemplo que mostra o que se pode fazer
 */
void do_movement_action(STATE *st, int dx, int dy) {
	st->playerX += dx;
	st->playerY += dy;
}

void update(STATE *st) {
	int key = getch();

	mvaddch(st->playerX, st->playerY, ' ');
	switch(key) {
		case KEY_A1:
		case '7': do_movement_action(st, -1, -1); break;
		case KEY_UP:
		case '8': do_movement_action(st, -1, +0); break;
		case KEY_A3:
		case '9': do_movement_action(st, -1, +1); break;
		case KEY_LEFT:
		case '4': do_movement_action(st, +0, -1); break;
		case KEY_B2:
		case '5': break;
		case KEY_RIGHT:
		case '6': do_movement_action(st, +0, +1); break;
		case KEY_C1:
		case '1': do_movement_action(st, +1, -1); break;
		case KEY_DOWN:
		case '2': do_movement_action(st, +1, +0); break;
		case KEY_C3:
		case '3': do_movement_action(st, +1, +1); break;
		case 'q': endwin(); exit(0); break;
	}
}


int mapa_teste(WINDOW *wnd) {
	STATE st = {20,20};
	int ncols, nrows;
	getmaxyx(wnd,nrows,ncols);
	
	// Criação e inicialização do mapa 
	NUM_COLUMNS = ncols;
	
	MAP (*mp)[ncols] = malloc(sizeof(MAP[nrows][ncols]));
    if (mp == NULL) {
        fprintf(stderr, "Erro: nao foi possivel alocar memoria\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nrows; i++){
      for (int j = 0; j < ncols; j++){
         mp[i][j].object = 0;
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
    gen_map(mp,nrows,ncols/2);
	print_map(mp,nrows,ncols/2);
	getch();
	//
	
    //free(mp);

	endwin(); 

	
	/**
	 * Este código está muito mal escrito!
	 * Deveria existir uma função chamada draw_player!
	 *
	 * Se estamos a desenhar uma luz à volta do jogador
	 * deveria existir uma função chamada draw_light!
	 *
	 */
	
	while(1) {
		move(nrows - 1, 0);
		attron(COLOR_PAIR(COLOR_BLUE));
		printw("(%d, %d) %d %d", st.playerX, st.playerY, ncols, nrows);
		attroff(COLOR_PAIR(COLOR_BLUE));
		attron(COLOR_PAIR(COLOR_BLACK));
		mvaddch(st.playerX, st.playerY, 'S' | A_BOLD);
		attroff(COLOR_PAIR(COLOR_RED));
		attron(COLOR_PAIR(COLOR_BLUE));
		mvaddch(st.playerX - 1, st.playerY - 1, '.' | A_BOLD);
		mvaddch(st.playerX - 1, st.playerY + 0, '.' | A_BOLD);
		mvaddch(st.playerX - 1, st.playerY + 1, '.' | A_BOLD);
		mvaddch(st.playerX + 0, st.playerY - 1, '.' | A_BOLD);
		mvaddch(st.playerX + 0, st.playerY + 1, '.' | A_BOLD);
		mvaddch(st.playerX + 1, st.playerY - 1, '.' | A_BOLD);
		mvaddch(st.playerX + 1, st.playerY + 0, '.' | A_BOLD);
		mvaddch(st.playerX + 1, st.playerY + 1, '.' | A_BOLD);
        attroff(COLOR_PAIR(COLOR_YELLOW));
		move(st.playerX, st.playerY);
		update(&st);
	} 
	return 0;
}
