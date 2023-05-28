#ifndef BOMBH
#define BOMBH

#include "game_types.h"
#include "engine_types.h"

void explode_at_pos(Vector2D pos, int damage, World *world, GameState *state, int radius);

void play_explosion_animation(Vector2D pos, int frame);

void destroy_bomb(World *world, int bombIndex);

void update_bombs_timer(World *world, unsigned long elapsedMicroseconds, GameState *state);

void place_bomb(Vector2D pos, int damage, int radius, World *world, unsigned long time);

void draw_bomb(Bomb *bomb, Terminal *terminal);

#endif