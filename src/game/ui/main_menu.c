#include <ncurses.h>
#include "../../engine/ui/components.c"
#include "engine_types.h"

// Menu principal (temporário)
int main_menu(Terminal *terminal){
    int selection = -1;
    while(selection == -1){

        // Título

        attron(A_BOLD);
        mvprintw(terminal->yMax / 2, terminal->xMax / 2 - 6, "PROJETO DE LI2");
        attroff(A_BOLD);

        attron(A_DIM);
        mvprintw(terminal->yMax / 2 + 1, terminal->xMax / 2 - 7, "(menu temporario)");
        attroff(A_DIM);

        // Input de seleção

        char *options[3] = { "JOGAR", "OPCOES", "SAIR"};
        int selection = menu_select(3, options, terminal->xMax - 12, terminal->yMax - 8, 5);
        
        if(selection == 2){
            // Confirmação no caso de saída
            if(modal_confim("Tem a certeza que quer sair?", 35, terminal->yMax, terminal->xMax)){
                return selection;
            }
        }
    }

    return selection;
}