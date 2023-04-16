#include <ncurses.h>
#include "main.h"
#include "ui/main_menu.c"


int main(){

    // Setup do ncurses
    initscr();

    // Certifica-se que o terminal onde o jogo está a rodar suporta a utilização de cores
    if(!can_change_color()){
        printw("Terminal não suporta cores.");
        getch();
        return -1;
    }

    // Faz com que o cursor fique invisível durante o jogo
    curs_set(0);

    start_color();

    // Obtem tamanho do terminal
    Terminal terminal;
    getmaxyx(stdscr, terminal.yMax, terminal.xMax);

    main_menu(&terminal);

    // Finaliza o ncurses
    endwin();
    return 0;
}