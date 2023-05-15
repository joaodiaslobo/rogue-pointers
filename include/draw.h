#ifndef DRAW
#define DRAW

#include "engine_types.h"

int draw_to_screen(Image image, Vector2D position);

int draw_to_window(WINDOW *window, Image image, Vector2D position);

void draw_custom_pixel(Vector2D position, char shapes[3], short primaryColor, short secondaryColor, Terminal *terminal);

void draw_empty_pixel(Vector2D position, short color);

#endif