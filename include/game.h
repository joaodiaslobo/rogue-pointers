#ifndef GAME
#define GAME

#include "game_types.h"
#include "map.h"

//extern int NUM_COLUMNS;
extern int LEVEL;

GameState *init_game_state();

void execute_input(GameState *state, World *w, int r, int c);

void check_for_portal(GameState *state, World *w, int r, int c, int dir);

void update(GameState *state, World *w, int r, int c) ;

int game(Terminal *terminal);

#endif