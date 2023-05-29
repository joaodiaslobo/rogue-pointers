#ifndef COMPONENTS
#define COMPONENTS

#include "engine_types.h"

int menu_select(int options, char *texts[], int width, int y, int x);

int modal_confim(char text[], int width, int screenYMax, int screenXMax);

void button(short color_gradient[4], char *text, Vector2D pos);

void progress_bar(int value, int max, int width, short primaryColor, short secondaryColor, char *text, Vector2D pos);

void text_input_box(Vector2D pos, int width, int inputSize, char *text, char *input);

#endif