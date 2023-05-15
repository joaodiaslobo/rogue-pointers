#ifndef PLAYER
#define PLAYER

#include "game_types.h"
#include "game.h"

Player *init_player(char name[15], Vector2D pos);

void apply_movement(GameState *gameState, Direction facing, Map** map, int r, int c);

void update_drowning(Map** map, GameState *gameState, unsigned long elapsedMicroseconds);

void draw_light(GameState *gameState, int r, int c, Map **map);

int light_before_walls(Vector2D posA, Vector2D posB, int distance, Map** map);

#endif