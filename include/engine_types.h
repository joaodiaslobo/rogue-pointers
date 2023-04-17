#ifndef HEADER_FILE
#define HEADER_FILE

#define MAX_IMAGE_PIXELS 300

// Controlo do terminal

typedef struct {
    int yMax;
    int xMax;
} Terminal;


// Math

typedef struct {
    int x;
    int y;
} Vector2D;

// Desenhar no terminal

typedef struct {
    int r;
    int g;
    int b;
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
