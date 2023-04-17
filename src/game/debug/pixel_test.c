#include <ncurses.h>
#include "game_types.h"
#include "engine_types.h"
#include "../../engine/render/draw.c"
#include "../../engine/render/image.c"

// Função de teste que desenha uma imagem no ecrã
int draw_mario(){
    Image mario = load_image_from_file("assets/sprites/characters/mario_test.sprite");

    Vector2D pos = {0,0};

    draw_to_screen(mario, pos);

    int key = 0;
    while(1){
        key = getch();
        switch (key)
        {
            case 65:
                pos.y--;
                break;
            case 66:
                pos.y++;
                break;
            case 67:
                pos.x++;
                break;
            case 68:
                pos.x--;
                break;
            default:
                break;
        }
        clear();
        draw_to_screen(mario, pos);
        mvprintw(0,0, "(%d, %d)", pos.x, pos.y);
    }

    return 0;
}

int pixel_test(){
    draw_mario();
    return 0;
}
