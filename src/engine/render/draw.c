#include <ncurses.h>
#include "engine_types.h"

// Desenha uma imagem na tela do terminal
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