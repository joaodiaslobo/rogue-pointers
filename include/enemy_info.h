#ifndef ENEMY_INFO
#define ENEMY_INFO

#include "engine_types.h"
#include "game_types.h"

void enemy_info_ui(GameState *state, World *world, Vector2D pos, Terminal *terminal);

void clean_ui();

#endif