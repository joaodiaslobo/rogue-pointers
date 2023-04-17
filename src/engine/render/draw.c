#include <ncurses.h>
#include "engine_types.h"

// Desenha uma imagem na tela do terminal
int draw_to_screen(Image image, Vector2D position){
    for(int i = 0; i < image.size; i++){
        int x = image.pixels[i].position.x * 2 + position.x * 2;
        int y = image.pixels[i].position.y + position.y;
        init_pair(i, COLOR_BLACK, image.pixels[i].color);
        attron(COLOR_PAIR(i));
        mvprintw(y, x, "  ");
        attroff(COLOR_PAIR(i));
    }
    return 0;
}   

// Cria uma imagem através do tamanho e pixels recebidos
Image create_image(int size, Pixel pixels[]) {
    Image image;
    image.size = size;
    for (int i = 0; i < size; i++) {
        image.pixels[i] = pixels[i];
    }
    return image;
}