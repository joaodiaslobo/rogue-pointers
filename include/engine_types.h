#ifndef ENGINE_TYPES
#define ENGINE_TYPES

#define MAX_IMAGE_PIXELS 300

// Controlo do terminal

typedef struct {
    int yMax;
    int xMax;
    WINDOW *mainWindow;
} Terminal;

// Math

typedef struct {
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

typedef struct {
    int red;
    int green;
    int blue;
} Color;

typedef struct {
    Vector2D position;
    short color;
} Pixel;

typedef struct {
    int size;
    Pixel pixels[MAX_IMAGE_PIXELS];
} Image;

#endif
