#include <ncurses.h>
#include "game_types.h"
#include "engine_types.h"
#include "../../engine/render/draw.c"

// Função de teste que desenha uma imagem no ecrã
int draw_mario(){
    Pixel pixels[144] = {
        {{3,0}, COLOR_RED},
        {{4,0}, COLOR_RED},
        {{5,0}, COLOR_RED},
        {{6,0}, COLOR_RED},
        {{7,0}, COLOR_RED},
        {{2,1}, COLOR_RED},
        {{3,1}, COLOR_RED},
        {{4,1}, COLOR_RED},
        {{5,1}, COLOR_RED},
        {{6,1}, COLOR_RED},
        {{7,1}, COLOR_RED},
        {{8,1}, COLOR_RED},
        {{9,1}, COLOR_RED},
        {{10,1}, COLOR_RED},
        {{2,2}, COLOR_CYAN},
        {{3,2}, COLOR_CYAN},
        {{4,2}, COLOR_CYAN},
        {{5,2}, COLOR_WHITE},
        {{6,2}, COLOR_WHITE},
        {{7,2}, COLOR_BLACK},
        {{8,2}, COLOR_WHITE},
        {{1,3}, COLOR_CYAN},
        {{2,3}, COLOR_WHITE},
        {{3,3}, COLOR_CYAN},
        {{4,3}, COLOR_WHITE},
        {{5,3}, COLOR_WHITE},
        {{6,3}, COLOR_WHITE},
        {{7,3}, COLOR_BLACK},
        {{8,3}, COLOR_WHITE},
        {{9,3}, COLOR_WHITE},
        {{10,3}, COLOR_WHITE},
        {{1,4}, COLOR_CYAN},
        {{2,4}, COLOR_WHITE},
        {{3,4}, COLOR_CYAN},
        {{4,4}, COLOR_CYAN},
        {{5,4}, COLOR_WHITE},
        {{6,4}, COLOR_WHITE},
        {{7,4}, COLOR_WHITE},
        {{8,4}, COLOR_BLACK},
        {{9,4}, COLOR_WHITE},
        {{10,4}, COLOR_WHITE},
        {{11,4}, COLOR_WHITE},
        {{1,5}, COLOR_CYAN},
        {{2,5}, COLOR_CYAN},
        {{3,5}, COLOR_WHITE},
        {{4,5}, COLOR_WHITE},
        {{5,5}, COLOR_WHITE},
        {{6,5}, COLOR_WHITE},
        {{7,5}, COLOR_BLACK},
        {{8,5}, COLOR_BLACK},
        {{9,5}, COLOR_BLACK},
        {{10,5}, COLOR_BLACK},
        {{3,6}, COLOR_WHITE},
        {{4,6}, COLOR_WHITE},
        {{5,6}, COLOR_WHITE},
        {{6,6}, COLOR_WHITE},
        {{7,6}, COLOR_WHITE},
        {{8,6}, COLOR_WHITE},
        {{9,6}, COLOR_WHITE},
        {{2,7}, COLOR_RED},
        {{3,7}, COLOR_RED},
        {{4,7}, COLOR_BLUE},
        {{5,7}, COLOR_RED},
        {{6,7}, COLOR_RED},
        {{7,7}, COLOR_RED},
        {{8,7}, COLOR_RED},
        {{1,8}, COLOR_RED},
        {{2,8}, COLOR_RED},
        {{3,8}, COLOR_RED},
        {{4,8}, COLOR_BLUE},
        {{5,8}, COLOR_RED},
        {{6,8}, COLOR_RED},
        {{7,8}, COLOR_BLUE},
        {{8,8}, COLOR_RED},
        {{9,8}, COLOR_RED},
        {{10,8}, COLOR_RED},
        {{0,9}, COLOR_RED},
        {{1,9}, COLOR_RED},
        {{2,9}, COLOR_RED},
        {{3,9}, COLOR_RED},
        {{4,9}, COLOR_BLUE},
        {{5,9}, COLOR_BLUE},
        {{6,9}, COLOR_BLUE},
        {{7,9}, COLOR_BLUE},
        {{8,9}, COLOR_RED},
        {{9,9}, COLOR_RED},
        {{10,9}, COLOR_RED},
        {{11,9}, COLOR_RED},
        {{0,10}, COLOR_WHITE},
        {{1,10}, COLOR_WHITE},
        {{2,10}, COLOR_RED},
        {{3,10}, COLOR_BLUE},
        {{4,10}, COLOR_YELLOW},
        {{5,10}, COLOR_BLUE},
        {{6,10}, COLOR_BLUE},
        {{7,10}, COLOR_YELLOW},
        {{8,10}, COLOR_BLUE},
        {{9,10}, COLOR_RED},
        {{10,10}, COLOR_WHITE},
        {{11,10}, COLOR_WHITE},
        {{0,11}, COLOR_WHITE},
        {{1,11}, COLOR_WHITE},
        {{2,11}, COLOR_WHITE},
        {{3,11}, COLOR_BLUE},
        {{4,11}, COLOR_BLUE},
        {{5,11}, COLOR_BLUE},
        {{6,11}, COLOR_BLUE},
        {{7,11}, COLOR_BLUE},
        {{8,11}, COLOR_BLUE},
        {{9,11}, COLOR_WHITE},
        {{10,11}, COLOR_WHITE},
        {{11,11}, COLOR_WHITE},
        {{0,12}, COLOR_WHITE},
        {{1,12}, COLOR_WHITE},
        {{2,12}, COLOR_BLUE},
        {{3,12}, COLOR_BLUE},
        {{4,12}, COLOR_BLUE},
        {{5,12}, COLOR_BLUE},
        {{6,12}, COLOR_BLUE},
        {{7,12}, COLOR_BLUE},
        {{8,12}, COLOR_BLUE},
        {{9,12}, COLOR_BLUE},
        {{10,12}, COLOR_WHITE},
        {{11,12}, COLOR_WHITE},
        {{2,13}, COLOR_BLUE},
        {{3,13}, COLOR_BLUE},
        {{4,13}, COLOR_BLUE},
        {{7,13}, COLOR_BLUE},
        {{8,13}, COLOR_BLUE},
        {{9,13}, COLOR_BLUE},
        {{1,14}, COLOR_CYAN},
        {{2,14}, COLOR_CYAN},
        {{3,14}, COLOR_CYAN},
        {{8,14}, COLOR_CYAN},
        {{9,14}, COLOR_CYAN},
        {{10,14}, COLOR_CYAN},
        {{0,15}, COLOR_CYAN},
        {{1,15}, COLOR_CYAN},
        {{2,15}, COLOR_CYAN},
        {{3,15}, COLOR_CYAN},
        {{8,15}, COLOR_CYAN},
        {{9,15}, COLOR_CYAN},
        {{10,15}, COLOR_CYAN},
        {{11,15}, COLOR_CYAN},
    };

    Image mario = create_image(144, pixels);

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
