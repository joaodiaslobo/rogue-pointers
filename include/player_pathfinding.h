#ifndef PLAYER_PATHFINDING
#define PLAYER_PATHFINDING

#include "engine_types.h"
#include "game_types.h"

void apply_path_change(GameState *gameState, Direction facing, MAP** map, int r, int c);

void apply_mouse_path_selection(GameState *gameState, MAP **map, Vector2D clickPos, int r, int c);

void draw_path(GameState *gameState);

int is_cell_path_part(GameState *gameState, Vector2D pos);

void update_player_path(GameState *gameState, unsigned long elapsedMicroseconds);

#endif