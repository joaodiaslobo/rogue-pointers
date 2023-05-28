#include <ncurses.h>
#include "inventory_ui.h"
#include "game_types.h"
#include "engine_types.h"
#include <math.h>

int show_items(Inventory *inventory){  
    int x = 0;

    for(int i = 1; i <= 10; i++){
        if(INVENTORY_SLOTS % i == 0){
            x = i;
        }
    }
    
    int k = 0;
    for(int i = 0; i < (INVENTORY_SLOTS)/x; i++){
        for(int j = 0; j < x; j++){
            if(inventory->items[k].type != NONE){
                attron(A_BOLD);
                attron(COLOR_PAIR(COLOR_WHITE));
                mvprintw(i + i * 25, j + j * 25, "%s", inventory->items[k].name);
                attroff(COLOR_PAIR(COLOR_WHITE));
                attroff(A_BOLD);
            }
            k++;
        }
    }

    return 0;
}