#ifndef GAME
#define GAME

#include "game_types.h"
#include "map.h"

extern int NUM_COLUMNS;

GameState *init_game_state();

void execute_input(GameState *state, MAP** m, int r, int c);

void check_for_portal(GameState *state, MAP** m, int r, int c);

void update(GameState *state, MAP** m, int r, int c) ;

int game(Terminal *terminal);

#endif