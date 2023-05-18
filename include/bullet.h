#ifndef BULLET
#define BULLET

#include "game_types.h"
#include "engine_types.h"

void shoot_bullet(Vector2D pos, Vector2D target, int damage, World *world);

void update_bullet(Bullet *bullet, Map **map, int bulletIndex, World *world, GameState *state);

void destroy_bullet(World *world, int bulletIndex);

void draw_bullet(Bullet *bullet);

#endif