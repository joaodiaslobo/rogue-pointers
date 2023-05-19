#include "engine_types.h"
#include "game_types.h"
#include <math.h>
#include "draw.h"
#include "image.h"
#include <stdlib.h>
#include "mobs_ai.h"
#include "a_star_pathfinding.h"
#include "enemy_info.h"
#include "bullet.h"

void update_timer(Mob *mob, unsigned long elapsedMicroseconds){
    mob->timeSinceLastUpdate += elapsedMicroseconds;
}

void wander_ai(Mob *mob, Player *player, Map** map, int r, int c){
    if(mob->timeSinceLastUpdate > 1000000 ||(mob->chasingPlayer && mob->timeSinceLastUpdate > 250000)){
        mob->timeSinceLastUpdate = 0;
        if(!can_see_location(mob->position, player->position, 13, map) && !mob->chasingPlayer){
            // Não vê player e não está em perseguição (modo de patrulha)
            if(mob->position.x == mob->targetPosition.x && mob->position.y == mob->targetPosition.y){
                // Quando já chegou à posição de patrulha que queria
                mob->targetPosition = pick_random_patrol_position(mob->position, map);
            } else {
                // Ainda não chegou à posição de patrulha? Move-se para a próxima "casa"
                mob->position = get_next_patrol_path_position(mob->position, mob->targetPosition);
            }
        } else if((!mob->chasingPlayer || (distance_between_points(mob->path[0], player->position) > 1.5f)) && can_see_location(mob->position, player->position, 13, map)){
            // Caso esteja a ver o player mas ainda não o esteja a perseguir ou caso esteja a perseguir o player mas este tenha entretanto mudado de posição
            Node *nodes = map_to_node_system(map, r, c);
            int steps;
            mob->path = find_path(nodes, player->position, mob->position, c, r, &steps);
            if(mob->path != NULL){
                mob->pathStep = steps - 1;
                mob->chasingPlayer = 1;

                // Reset alvo de patrulha para prevenir movimentos incorretos
                mob->targetPosition = mob->path[0];
            }


            if(mob->pathStep >= 0){
                mob->position = mob->path[mob->pathStep];
                mob->pathStep--;
            }

            apply_damage(mob, player);
        } else {
            // Em perseguição
            if(mob->pathStep >= 0){
                mob->position = mob->path[mob->pathStep];
                mob->pathStep--;
            } else {
                mob->chasingPlayer = 0;
                free(mob->path);
            }

            apply_damage(mob, player);
        }
    }
}

void tactical_ai(Mob *mob, Player *player, Map** map, World *world){
    if(mob->timeSinceLastUpdate > 1000000 ||(mob->chasingPlayer && mob->timeSinceLastUpdate > 250000)){
        mob->timeSinceLastUpdate = 0;
        if(!can_see_location(mob->position, player->position, 13, map) && !mob->chasingPlayer){
            // Não vê player e não está em perseguição (modo de patrulha)
            if(mob->position.x == mob->targetPosition.x && mob->position.y == mob->targetPosition.y){
                // Quando já chegou à posição de patrulha que queria
                mob->targetPosition = pick_random_patrol_position(mob->position, map);
            } else {
                // Ainda não chegou à posição de patrulha? Move-se para a próxima "casa"
                mob->position = get_next_patrol_path_position(mob->position, mob->targetPosition);
            }
        } else {
            // Se estiver a ver o jogador dispara
            shoot_bullet(mob->position, player->position, mob->attackDamage, world);
        }
    }
}

void remove_enemy_from_world(World *world, int enemyIndex){
    if(world->mobQuantity > 1){
        world->mobs[enemyIndex] = world->mobs[world->mobQuantity - 1];
        world->mobs = realloc(world->mobs, sizeof(Mob) * (world->mobQuantity - 1));
    } else {
        free(world->mobs);
    }
    
    // Reset mob stats UI
    clean_ui();

    world->mobQuantity--;
}

void apply_damage(Mob *mob, Player *player){
    // Se o mob estiver a uma distância suficiente de ataque, ataca o player
    if(distance_between_points(mob->position, player->position) <= 1.5f){
        player->health -= mob->attackDamage;
    }
}

Vector2D get_next_patrol_path_position(Vector2D pos, Vector2D target){
    Vector2D newPos = {0,0};
    if(pos.x > target.x){
        newPos.x = pos.x - 1;
        newPos.y =  pos.y;
        return newPos;
    }
    else if(pos.x < target.x){
        newPos.x = pos.x + 1;
        newPos.y = pos.y;
        return newPos;
    }
    else if(pos.y < target.y){
        newPos.x = pos.x; 
        newPos.y = pos.y + 1;
        return newPos;
    }
    else if(pos.y > target.y){
        newPos.x = pos.x;
        newPos.y = pos.y - 1;
        return newPos;
    }

    // Erro
    return pos;
}

float distance_between_points(Vector2D a, Vector2D b){
    return sqrt( (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) );
}

// Algoritmo de Bresenham, verifica a visibilidade entre a posição do jogador e a posição do inimigo
int can_see_location(Vector2D posA, Vector2D posB, int distance, Map** map){
    int dx = abs(posB.x - posA.x);
    int dy = abs(posB.y - posA.y);
    int sx, sy;

    if(posA.x < posB.x){
        sx = 1;
    } else {
        sx = -1;
    }

    if(posA.y < posB.y){
        sy = 1;
    } else {
        sy = -1;
    }

    int err = dx - dy;
     
    Vector2D pos = {posA.x, posA.y};
    
    while ((pos.x != posB.x || pos.y != posB.y) && (map[pos.y][pos.x].object != 1) && (map[pos.y][pos.x].object != 3) && (distance > 0))
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

        distance--;
    }

    if(pos.x == posB.x && pos.y == posB.y){
        return 1;
    }
    return 0;
}

Vector2D pick_random_patrol_position(Vector2D pos, Map **map){
    // Primeiro, escolhe uma direção aleatória
    Direction direction = random() % 4;
    // Descobre quantas casas pode andar nessa direção
    int availableInDirection = available_floor_in_direction(pos, map, direction);
    while(availableInDirection == 0){
        direction = random() % 4;
        availableInDirection = available_floor_in_direction(pos, map, direction);
    }
    int cellsToMove = (random() % availableInDirection) + 1;
    Vector2D newPos = {0,0};
    switch(direction){
        case NORTH:
            newPos.x = pos.x;
            newPos.y = pos.y - cellsToMove;
            return newPos;
        case SOUTH:
            newPos.x = pos.x;
            newPos.y = pos.y + cellsToMove;
            return newPos;
        case EAST:
            newPos.x = pos.x + cellsToMove;
            newPos.y = pos.y;
            return newPos;
        case WEST:
            newPos.x = pos.x - cellsToMove; 
            newPos.y = pos.y;
            return newPos;
        default:
            return newPos;
    }
}

int available_floor_in_direction(Vector2D pos, Map **map, Direction direction){
    int distance = 0;
    switch (direction)
    {
        case NORTH:
            for(distance = 0; map[pos.y - distance][pos.x].object != 1; distance++);
            return distance - 1;
            break;
        case SOUTH:
            for(distance = 0; map[pos.y + distance][pos.x].object != 1; distance++);
            return distance - 1;
            break;
        case EAST:
            for(distance = 0; map[pos.y][pos.x + distance].object != 1; distance++);
            return distance - 1;
            break;
        case WEST:
            for(distance = 0; map[pos.y][pos.x - distance].object != 1; distance++);
            return distance - 1;
            break;
        default:
            break;
    }
    return distance;
}