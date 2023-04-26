#include <ncurses.h>
#include "engine_types.h"
#include "ui/main_menu.c"
#include "debug/pixel_test.c"
#include "../engine/render/color.c"
#include "game.c"

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

    nodelay(stdscr, TRUE);
	meta(stdscr, TRUE);
	keypad(stdscr, TRUE);

    start_color();

    // Carrega a palette para o ncurses
    if(!load_palette_from_file("assets/main.palette")){
        printw("Falha no carregamento da palette.");
    }

    // Obtem tamanho do terminal
    Terminal terminal;
    getmaxyx(stdscr, terminal.yMax, terminal.xMax);
	terminal.mainWindow = window;

    int selection = main_menu(&terminal);
    switch (selection)
    {
    case 3:
        pixel_test();
        break;
	case 0:
		game_loop(&terminal);
		break;
    default:
        break;
    }

    // Finaliza o ncurses
    endwin();
    return 0;
}
