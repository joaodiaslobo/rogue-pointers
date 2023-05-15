#ifndef DRAW
#define DRAW

#include "engine_types.h"

int draw_to_screen(Image image, Vector2D position);

int draw_to_window(WINDOW *window, Image image, Vector2D position);

#endif