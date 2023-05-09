#include "game_types.h"

void gen_water(MAP** a, int r, int c);

void gen_grass(MAP** a, int r, int c);

void gen_lava(MAP** a, int r, int c);

void new_room_map(MAP** a, int r, int c);

int gen_mobs(Mob *mobs, MAP **map, int r, int c, int level);

Vector2D get_random_floor_position(MAP** map, int r, int c);

void gen_map(MAP** a, int r, int c);

void draw_mobs(Mob *mobs, int r, int c, int mobQuantity);

void print_map(MAP** a, int r, int c);
