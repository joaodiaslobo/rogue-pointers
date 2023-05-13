#ifndef GAME
#define GAME

#include "game_types.h"
#include "map.h"
#include "player.h"
#include "sys/time.h"

extern int LEVEL;
    
GameState *init_game_state();

void execute_input(GameState *state, World *w, int r, int c);

void check_for_portal(GameState *state, World *w, int r, int c, int dir);

void update(GameState *state, World *worlds, int r, int c, struct timeval currentTime);

int game(Terminal *terminal);

#endif