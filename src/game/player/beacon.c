#include "beacon.h"
#include "game_types.h"
#include "engine_types.h"
#include <stdlib.h>
#include "draw.h"

/*

* a104356 - João Lobo

* Coloca um beacon de luz no mapa.

*/
void place_beacon(Vector2D pos, World *world){
    world->beaconLocations = realloc(world->beaconLocations, sizeof(Vector2D) * (world->beacons + 1));
    world->beaconLocations[world->beacons] = pos;
    world->beacons++;
}

/*

* a104356 - João Lobo

* Desenha um beacon de luz.

*/
void draw_beacon(Vector2D pos, Terminal *terminal){
    draw_custom_pixel(pos, "||", 59, 96, terminal);
}