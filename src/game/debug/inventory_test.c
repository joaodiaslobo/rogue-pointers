#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "game_types.h"
#include "engine_types.h"
#include "inventory_ui.h"
#include "inventory_test.h"
#include "inventory.h"
#include "draw.h"
#include "image.h"

int inventory_test(){
    Item item1 = {"ESPADA", 1, "SJJHS"};
    Item item2 = {"ARCO", 1, "SJJHS"};
    Item item3 = {"POCAO DE VIDA", 1, "SJJHS"};
    Item item4 = {"BIFE", 1, "SJJHS"};
    Inventory inv = {{item1, item2, item3, item4}};
    erase();
    while(1){
        getch();
        show_items(&inv);
    }
    return 0;
}