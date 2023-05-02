#include "engine_types.h"
#include <math.h>
#include "draw.h"
#include "image.h"

// y = mx + b
int can_see_location(Vector2D posA, Vector2D posB, int distance){
    int dx = abs(posA.x - posB.x);
    int dy = abs(posA.y - posB.y);
    int p = 2 * dy - dx;
    int x,y,end;
    if(posA.x > posB.x){
        x = posB.x;
        y = posB.y;
        end = posA.x;
    } else {
        x = posA.x;
        y = posA.y;
        end = posB.x;
    }
    Image teste = load_image_from_file("assets/sprites/grass.sprite");
    Vector2D pos = {x,y};
    draw_to_screen(teste, pos);
    while(x < end){
        x += 1;
        if(p < 0){
            p = p + x * dy;
        } else {
            y = y + 1;
            p = p + 2 * (dy - dx);
        }
        pos.x = x;
        pos.y = y;
        draw_to_screen(teste, pos);
    }
}