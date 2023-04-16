#include <ncurses.h>
#include <string.h>
#include "../utils/utils.c"

/*
    Elementos de UI genéricos, podem ser utilizados por si mesmos ou combinados com outros.
*/

// Função geradora de menus de opções, recebe o número de opções, o texto de cada opção, a largura da caixa (número de colunas), a sua posição y e a sua posição x 
int menu_select(int options, char *texts[], int width, int y, int x){
    int selection = 0, key = 0;
    int lines  = options + 4;

    WINDOW * selectWindow = newwin(lines, width, y, x);
    box(selectWindow, 0, 0);
    refresh();
    wrefresh(selectWindow);
    keypad(selectWindow, true);

    while (key != 10)
    {
        for(int i = 0; i < options; i++){
            if(i == selection){
                wattron(selectWindow, A_REVERSE);
                mvwprintw(selectWindow, i+2, 1, "[x] %s", texts[i]);
                wattroff(selectWindow, A_REVERSE);
            } else {
                mvwprintw(selectWindow, i+2, 1, "[ ] %s", texts[i]);
            }
            wrefresh(selectWindow);
        }

        move(0, 0);

        key = wgetch(selectWindow);
        switch (key)
        {
            case KEY_UP:
                if(selection > 0){
                    selection--;
                } else {
                    selection = options - 1;
                }
                break;
            case KEY_DOWN:
                if(selection < options - 1){
                    selection++;
                } else {
                    selection = 0;
                }
                break;
            default:
                break;
        }
    }

    wclear(selectWindow);
    wrefresh(selectWindow);
    delwin(selectWindow);
    return selection;
}

// Função geradora de uma caixa de confimação, recebe o texto da caixa, a sua largura, a altura da tela do terminal e a largura da tela do terminal
int modal_confim(char text[], int width, int screenYMax, int screenXMax){

    int key = 0, option = 0;
    int necessaryLines = count_newlines(text) + 1 + 4;
    int posY = (screenYMax - necessaryLines) / 2;
    int posX = (screenXMax - width) / 2;

    WINDOW * modalWindow = newwin(necessaryLines, width, posY, posX);
    box(modalWindow, 0, 0);
    refresh();
    wrefresh(modalWindow);
    keypad(modalWindow, true);

    mvwprintw(modalWindow, 1, 2, "%s", text);


    while(key != 10){
        if(!option){
            wattron(modalWindow, A_REVERSE);
            mvwprintw(modalWindow, necessaryLines - 2, 2, "SIM");
            wattroff(modalWindow, A_REVERSE);
            mvwprintw(modalWindow, necessaryLines - 2, width - 5, "NAO");
        } else {
            wattron(modalWindow, A_REVERSE);
            mvwprintw(modalWindow, necessaryLines - 2, width - 5, "NAO");
            wattroff(modalWindow, A_REVERSE);
            mvwprintw(modalWindow, necessaryLines - 2, 2, "SIM");
        }

        key = wgetch(modalWindow);
        switch (key)
        {
            case KEY_RIGHT:
                if(option){
                    option--;
                } else {
                    option++;
                }
                break;
            case KEY_LEFT:
                if(option){
                    option--;
                } else {
                    option++;
                }
                break;
            default:
                break;
        }
    }

    wclear(modalWindow);
    wrefresh(modalWindow);
    delwin(modalWindow);
    return !option;
}