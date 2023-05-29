#ifndef BEACON
#define BEACON

#include "engine_types.h"
#include "game_types.h"

void place_beacon(Vector2D pos, World *world);

void draw_beacon(Vector2D pos, Terminal *terminal);

#endif