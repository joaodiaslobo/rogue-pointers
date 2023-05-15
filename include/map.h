#ifndef MAP
#define MAP

#include "game_types.h"
#include "engine_types.h"

void gen_water(Map** a, int r, int c);

void gen_grass(Map** a, int r, int c);

void gen_lava(Map** a, int r, int c);

void new_room_map(Map** a, int r, int c);

int gen_mobs(Mob *mobs, Map **map, int r, int c, int level);

Vector2D get_random_floor_position(Map** map, int r, int c);

void gen_map(Map** a, int r, int c);

void draw_mobs(Mob *mobs, int mobQuantity);

void print_map(Map** a, int r, int c, GameState *gameState, Terminal *terminal);

#endif