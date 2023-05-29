#include "about_menu.h"
#include "components.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "sound.h"
#include <pthread.h>

/*

* a104439 - Rita Camacho

* Menu about do jogo.

*/

void show_about_menu(Terminal *terminal,  WINDOW *window){
    int key = 0;
    while(key != 10){
        wattron(window, A_BOLD);
        mvwprintw(window, terminal->yMax/5, terminal->xMax/2, "ABOUT");
        wattroff(window, A_BOLD);
        
        wattron(window, A_NORMAL);
        mvwprintw(window, terminal->yMax/5 + 4, terminal->xMax/2 - 194/2 + 1, "This game was developed by João Lobo (A104356), Rita Camacho (A104439), Sara Lopes (A104179) and Tomás Melo (A104529), Software Engineering & Computer Science students at University of Minho,");
        mvwprintw(window, terminal->yMax/5 + 5, terminal->xMax/2 - 117/2 + 1, "as part of the final project of the course \"Computer Labs II\" of the 1st year (2nd semester, academic year 2022/2023).");
        mvwprintw(window, terminal->yMax/5 + 6, terminal->xMax/2 - 121/2 + 1, "The aim of the final project is the creation of a \"Roguelike\" game using the ncurses library and C programming language.");
        mvwprintw(window, terminal->yMax/5 + 7, terminal->xMax/2 - 103/2 + 1, "For further details, visit the \"Rogue Pointers\" repository, published in GitHub, and our profiles:");
        mvwprintw(window, terminal->yMax/5 + 10, terminal->xMax/2 - 63/2 + 1, "@joaodiaslobo @ritacamacho @Zaninhazevedo @tomarshmallowwwww11");
        wattroff(window, A_NORMAL);

        wattron(window, A_BOLD);
        mvwprintw(window, terminal->yMax/2 + 2, terminal->xMax/2, "SOBRE");
        wattroff(window, A_BOLD);
        
        wattron(window, A_NORMAL);
        mvwprintw(window, terminal->yMax/2 + 6, terminal->xMax/2 - 174/2 + 1, "Este jogo foi desenvolvido por João Lobo (A104356), Rita Camacho (A104439), Sara Lopes (A104179) e Tomás Melo (A104529), alunos da licenciatura de Engenharia Informática,");
        mvwprintw(window, terminal->yMax/2 + 7, terminal->xMax/2 - 160/2 + 1, "na Universidade do Minho, no âmbito do projeto final da unidade curricular \"Laboratórios de Informática II\" do 1º ano (2º semestre, ano letivo 2022/2023).");
        mvwprintw(window, terminal->yMax/2 + 8, terminal->xMax/2 - 141/2 + 1, "O objetivo do projeto final é a criação dum jogo \"Roguelike\" utilizando a livraria ncurses juntamente com a linguagem de programação C.");
        mvwprintw(window, terminal->yMax/2 + 9, terminal->xMax/2 - 100/2 + 1, "Para mais detalhes, visite o repositório \"Rogue Pointers\" publicado no GitHub, e os nossos perfis:");
        mvwprintw(window, terminal->yMax/2 + 12, terminal->xMax/2 - 63/2 + 1, "@joaodiaslobo @ritacamacho @Zaninhazevedo @tomarshmallowwwww11");
        wattroff(window, A_NORMAL);

        wattron(window, A_BOLD);
        mvwprintw(window, terminal->yMax - 5, terminal->xMax/2 - 23/2 + 2, "Press [ENTER] to leave");
        wattroff(window, A_BOLD);

        key = wgetch(window);
    }
}