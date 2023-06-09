#ifndef GAME
#define GAME

#include "game_types.h"
#include "map.h"
#include "player.h"
#include "sys/time.h"

extern int LEVEL, num_levels;
    
GameState *init_game_state();

void execute_input(GameState *state, World *w, int r, int c, Terminal *terminal);

void check_for_portal(GameState *state, World *w, int r, int c, int dir);

void update(GameState *state, World *worlds, int r, int c, struct timeval currentTime, Terminal *terminal);

int game(Terminal *terminal, char *playerName);

#endif