#ifndef COLOR
#define COLOR

#include "engine_types.h"

int two_hexadecimal_to_int(char hex[]);

Color hex_to_color(char hex[]);

int add_color_to_palette(Color color, short index);

int load_palette_from_file(char *path, Terminal *terminal);

#endif