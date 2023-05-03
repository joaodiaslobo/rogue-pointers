#include "engine_types.h"
#include "game_types.h"
#include <math.h>
#include "draw.h"
#include "image.h"

// Algoritmo de Bresenham 
int can_see_location(Vector2D posA, Vector2D posB, int distance, MAP** map){
    Image teste = load_image_from_file("assets/sprites/grass.sprite");
    int dx = abs(posB.x - posA.x);
    int dy = abs(posB.y - posA.y);
    int sx = (posA.x < posB.x) ? 1 : -1;
    int sy = (posA.y < posB.y) ? 1 : -1;
    int err = dx - dy;
     
    Vector2D pos = {posA.x, posA.y};
    //draw_to_screen(teste, pos);
    
    while ((pos.x != posB.x || pos.y != posB.y) && (map[pos.y][pos.x].object != 1) && (distance > 0))
    {
        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            pos.x += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            pos.y += sy;
        }

        //draw_to_screen(teste, pos);
        distance--;
    }

    if(pos.x == posB.x && pos.y == posB.y){
        draw_to_screen(teste, posA);
    }
}