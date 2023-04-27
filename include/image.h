#ifndef IMAGE
#define IMAGE

#include "engine_types.h"

Image create_image(int size, Pixel pixels[]);

Image load_image_from_file(char path[]);

#endif