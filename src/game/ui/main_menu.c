#include <ncurses.h>
#include "components.h"
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

        attron(A_BOLD);
        mvprintw(terminal->yMax - 1, 5, "LINHAS: %d, COLUNAS: %d", terminal->yMax , terminal->xMax);
        attroff(A_BOLD);

        // Input de seleção
        char *options[] = { "JOGAR", "OPCOES", "SAIR", "DEBUG"};
        selection = menu_select(4, options, terminal->xMax - 12, terminal->yMax - 9, 5);
        
        if(selection == 2){
            // Confirmação no caso de saída
            if(modal_confim("Tem a certeza que quer sair?", 35, terminal->yMax, terminal->xMax)){
                return selection;
            }
            selection = -1;
        }
    }

    return selection;
}