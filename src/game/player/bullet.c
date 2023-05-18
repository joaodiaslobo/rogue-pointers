#include "game_types.h"
#include "engine_types.h"
#include "bullet.h"
#include "draw.h"
#include <stdlib.h>
#include "mobs_ai.h"

void shoot_bullet(Vector2D pos, Vector2D target, int damage, World *world){
    Bullet bullet;
    bullet.damage = damage;
    bullet.position = pos;
    bullet.target = target;

    world->bullets = realloc(world->bullets, sizeof(Bullet) * (world->bulletQuantity + 1));
    world->bullets[world->bulletQuantity] = bullet;
    world->bulletQuantity++;
}

void update_bullet(Bullet *bullet, Map **map, int bulletIndex, World *world, GameState *state){
    int dx = abs(bullet->position.x - bullet->target.x);
    int dy = abs(bullet->position.y - bullet->target.y);
    int sx, sy;

    if(bullet->position.x < bullet->target.x){
        sx = 1;
    } else {
        sx = -1;
    }

    if(bullet->position.y < bullet->target.y){
        sy = 1;
    } else {
        sy = -1;
    }

    Vector2D pos = {bullet->position.x, bullet->position.y};
    
    int err = dx - dy;

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

    if(map[pos.y][pos.x].object == 1 || map[pos.y][pos.x].object == 3){
        // Destruir bala quando "acerta" numa parede
        destroy_bullet(world, bulletIndex);
    } else if(state->player.position.x == pos.x && state->player.position.y == pos.y) {
        // Bala acerta no player
        state->player.health -= bullet->damage;
        destroy_bullet(world, bulletIndex);
    } else if(pos.x == bullet->target.x && pos.y == bullet->target.y){
        // Bala chegou à posição do click
        destroy_bullet(world, bulletIndex);
    }
    else {
        // Verificar se acerta em mobs
        int hasAttacked = 0;
        for(int i = 0; i < world->mobQuantity; i++){
            if(world->mobs[i].position.x == pos.x && world->mobs[i].position.y == pos.y && !hasAttacked){
                world->mobs[i].health -= bullet->damage;
                destroy_bullet(world, bulletIndex);

                if(world->mobs[i].health <= 0){
                    remove_enemy_from_world(world, i);
                }
                
                hasAttacked = 1;
            }
        }

        if(!hasAttacked){
            bullet->position = pos;
        }
    }
}

void destroy_bullet(World *world, int bulletIndex){
    if(world->bulletQuantity > 1){
        world->bullets[bulletIndex] = world->bullets[world->bulletQuantity - 1];
        world->bullets = realloc(world->bullets, sizeof(Bullet) * (world->bulletQuantity - 1));
    }
    world->bulletQuantity--;
}

void draw_bullet(Bullet *bullet){
    draw_empty_pixel(bullet->position, 1);
}