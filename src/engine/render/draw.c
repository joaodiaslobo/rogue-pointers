#include "draw.h"
#include <ncurses.h>
#include "engine_types.h"

/*

* a104356 - João Lobo

* Desenha uma imagem na tela do terminal numa dada posição.

*/
int draw_to_screen(Image image, Vector2D position){
    for(int i = 0; i < image.size; i++){
        int x = image.pixels[i].position.x * 2 + position.x * 2;
        int y = image.pixels[i].position.y + position.y;
        attron(COLOR_PAIR(image.pixels[i].color));
        mvprintw(y, x, "  ");
        attroff(COLOR_PAIR(image.pixels[i].color));
    }
    return 0;
}

/*

* a104356 - João Lobo

* Desenha uma imagem numa janela ncurses numa dada posição.

*/
int draw_to_window(WINDOW *window, Image image, Vector2D position){
    for(int i = 0; i < image.size; i++){
        int x = image.pixels[i].position.x * 2 + position.x * 2;
        int y = image.pixels[i].position.y + position.y;
        wattron(window, COLOR_PAIR(image.pixels[i].color));
        mvwprintw(window, y, x, "  ");
        wattroff(window, COLOR_PAIR(image.pixels[i].color));
    }
    return 0;
}

/*

* a104356 - João Lobo

* Desenha um pixel colorido numa dada posição.

*/
void draw_empty_pixel(Vector2D position, short color){
    attron(COLOR_PAIR(color + 8));
    mvprintw(position.y, position.x * 2, "  ");
    attroff(COLOR_PAIR(color + 8));
}

/*

* a104356 - João Lobo

* Desenha um pixel colorido com caracteres especiais numa dada posição.

*/
void draw_custom_pixel(Vector2D position, char shapes[3], short primaryColor, short secondaryColor, Terminal *terminal){
    // Verifica se já foi criado um par igual para poupar espaço
    short index = -1;
    for(int i = 0; i < terminal->loadedRuntimePairs && index == -1; i++){
        short foreground = 0;
        short background = 0;
        pair_content(i + terminal->loadedColors + 9, &foreground, &background);
        if(foreground == primaryColor + 8 && background == secondaryColor + 8){
            index = i + terminal->loadedColors + 9;
        }
    }

    // Caso este par customizado ainda não tenha sido criado, adicionamo-lo ao terminal
    if(index == -1){
        index = terminal->loadedColors + 9 + terminal->loadedRuntimePairs;
        init_pair(index, primaryColor + 8, secondaryColor + 8);
        terminal->loadedRuntimePairs++;
    }

    attron(COLOR_PAIR(index));
    mvprintw(position.y, position.x * 2, "%s", shapes);
    attroff(COLOR_PAIR(index));

    return;
}

/*

* a104356 - João Lobo

* Desenha um pixel colorido com caracteres especiais numa dada posição numa janela ncurses.

*/
void draw_custom_pixel_to_window(Vector2D position, char shapes[3], short primaryColor, short secondaryColor, Terminal *terminal, WINDOW *window){
    // Verifica se já foi criado um par igual para poupar espaço
    short index = -1;
    for(int i = 0; i < terminal->loadedRuntimePairs && index == -1; i++){
        short foreground = 0;
        short background = 0;
        pair_content(i + terminal->loadedColors + 9, &foreground, &background);
        if(foreground == primaryColor + 8 && background == secondaryColor + 8){
            index = i + terminal->loadedColors + 9;
        }
    }

    // Caso este par customizado ainda não tenha sido criado, adicionamo-lo ao terminal
    if(index == -1){
        index = terminal->loadedColors + 9 + terminal->loadedRuntimePairs;
        init_pair(index, primaryColor + 8, secondaryColor + 8);
        terminal->loadedRuntimePairs++;
    }

    wattron(window, COLOR_PAIR(index));
    mvwprintw(window, position.y, position.x * 2, "%s", shapes);
    wattroff(window, COLOR_PAIR(index));

    return;
}