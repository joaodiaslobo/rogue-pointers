#ifndef MOBS_AI
#define MOBS_AI

#include "engine_types.h"

void update_timer(Mob *mob, unsigned long elapsedMiliseconds);

void wander_ai(Mob *mob, Player *player, MAP** map);

Vector2D get_next_patrol_path_position(Vector2D pos, Vector2D target);

int can_see_location(Vector2D posA, Vector2D posB, int distance, MAP** map);

Vector2D pick_random_patrol_position(Vector2D pos, MAP **map);

int available_floor_in_direction(Vector2D pos, MAP **map, Direction direction);

#endif