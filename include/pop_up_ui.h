#ifndef POP_UP_UI
#define POP_UP_UI

#include <string.h>
#include "utils.h"
#include "engine_types.h"
#include "game.h"
#include "main_menu.h"

int new_items_pop_up(char text[], int width, int screenYMax, int screenXMax);

int winner_pop_up(char text[], int width, int screenYMax, int screenXMax, Terminal *terminal);

int game_over_pop_up(char text[], int width, int screenYMax, int screenXMax, Terminal *terminal);

void help_pop_up(Terminal *terminal);

int pause_pop_up(char text[], int width, int screenYMax, int screenXMax, Terminal *terminal);

#endif