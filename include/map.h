#ifndef MAP
#define MAP

#include "game_types.h"

extern int NUM_COLUMNS;

void new_room_map (Map (*a)[NUM_COLUMNS], int r, int c);

void new_level_map (Map (*a)[NUM_COLUMNS], int r, int c);

void gen_border_map(Map (*a)[NUM_COLUMNS], int r, int c);

void gen_first_map(Map (*a)[NUM_COLUMNS], int r, int c);

void smooth_map(Map (*a)[NUM_COLUMNS], int r, int c, int x1, int x2);

void gen_map(Map (*a)[NUM_COLUMNS], int r, int c, int i);

void print_map(Map (*a)[NUM_COLUMNS], int r, int c);

#endif