#ifndef ENGINE_TYPES
#define ENGINE_TYPES

#include <ncurses.h>

#define MAX_IMAGE_PIXELS 500


// Controlo do terminal

typedef struct terminal {
    int yMax;
    int xMax;
    WINDOW *mainWindow;
    short loadedColors;
    short loadedRuntimePairs;
} Terminal;

// Math

typedef struct vector2d {
    int x;
    int y;
} Vector2D;

typedef enum {
    NORTH = 0,
    SOUTH = 1,
    EAST = 2,
    WEST = 3
} Direction;

// Desenhar no terminal

typedef struct color {
    int red;
    int green;
    int blue;
} Color;

typedef struct pixel {
    Vector2D position;
    short color;
} Pixel;

typedef struct image {
    int size;
    Pixel pixels[MAX_IMAGE_PIXELS];
} Image;

#endif
