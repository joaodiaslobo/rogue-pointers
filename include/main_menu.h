#ifndef MAIN_MENU
#define MAIN_MENU

#include "engine_types.h"

int main_menu(Terminal *terminal);

int main_menu_update(int options, char *texts[], int width, int y, int x, Terminal *terminal);

void play_move_sound();

void create_sparkles_animation(Vector2D pos, int frame, Terminal *terminal);

#endif