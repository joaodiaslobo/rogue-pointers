#ifndef MOBS_AI
#define MOBS_AI

#include "engine_types.h"

void update_timer(Mob *mob, unsigned long elapsedMiliseconds);

void wander_ai(Mob *mob, Player *player, Map** map, int r, int c);

void tactical_ai(Mob *mob, Player *player, Map** map, World *world);

void apply_damage(Mob *mob, Player *player);

Vector2D get_next_patrol_path_position(Vector2D pos, Vector2D target);

float distance_between_points(Vector2D a, Vector2D b);

int can_see_location(Vector2D posA, Vector2D posB, int distance, Map** map);

Vector2D pick_random_patrol_position(Vector2D pos, Map **map);

void remove_enemy_from_world(World *world, int enemyIndex);

int available_floor_in_direction(Vector2D pos, Map **map, Direction direction);

#endif