#ifndef PLAYER
#define PLAYER

#include "game_types.h"
#include "game.h"

Player *init_player(char name[15], Vector2D pos);

void apply_movement(GameState *gameState, Direction facing, MAP** map, int r, int c);

void draw_light(GameState *gameState, int r, int c);

#endif