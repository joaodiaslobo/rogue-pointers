#ifndef INVENTORY_MENU
#define INVENTORY_MENU

#include "game_types.h"
#include "engine_types.h"

void show_inventory(Terminal *terminal, WINDOW * inventoryWindow, GameState *state);

void clear_item(Terminal *terminal, WINDOW *window);

void item_description(Terminal *terminal, WINDOW *window, char description[]);

#endif