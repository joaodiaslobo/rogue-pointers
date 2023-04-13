#include <ncurses.h>

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
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    attron(A_BOLD);
    mvprintw(yMax / 2, xMax / 2 - 6, "PROJETO DE LI2");
    attroff(A_BOLD);

    attron(A_DIM);
    mvprintw(yMax / 2 + 1, xMax / 2 - 7, "(menu temporario)");
    attroff(A_DIM);

    // Menu principal (temporário)

    WINDOW * menuWindow = newwin(7, xMax-12, yMax-8, 5);
    box(menuWindow, 0, 0);
    refresh();
    wrefresh(menuWindow);
    keypad(menuWindow, true);

    char options[3][10] = { "JOGAR", "OPCOES", "SAIR" };
    int key = 0;
    int current = 0;

    while(1){
        for(int i = 0; i < 3; i++){
            if(i == current){
                wattron(menuWindow, A_REVERSE);
                mvwprintw(menuWindow, i+2, 1, "[x] %s", options[i]);
                wattroff(menuWindow, A_REVERSE);
            } else {
                mvwprintw(menuWindow, i+2, 1, "[ ] %s", options[i]);
            }
        }

        move(0, 0);

        key = wgetch(menuWindow);
        switch (key)
        {
            case KEY_UP:
                if(current > 0){
                    current--;
                } else {
                    current = 2;
                }
                break;
            case KEY_DOWN:
                if(current < 2){
                    current++;
                } else {
                    current = 0;
                }
                break;
            default:
                break;
        }

        if(key == 10){
            break;
        }
    }

    // Opção escolhida está no int current
    printw("ESCOLHA: %s", options[current]);

    // Finaliza o ncurses
    getch();
    endwin();

    return 0;
}