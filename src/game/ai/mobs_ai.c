#include "engine_types.h"
#include "game_types.h"
#include <math.h>
#include "draw.h"
#include "image.h"
#include <stdlib.h>
#include "mobs_ai.h"
#include "a_star_pathfinding.h"

void update_timer(Mob *mob, unsigned long elapsedMicroseconds){
    mob->timeSinceLastUpdate += elapsedMicroseconds;
}

void wander_ai(Mob *mob, Player *player, MAP** map, int r, int c){
    if(mob->timeSinceLastUpdate > 1000000){
        mob->timeSinceLastUpdate = 0;
        if(!can_see_location(mob->position, player->position, 13, map)){
            if(mob->position.x == mob->targetPosition.x && mob->position.y == mob->targetPosition.y){
                // Quando já chegou à posição de patrulha que queria
                mob->targetPosition = pick_random_patrol_position(mob->position, map);
            } else {
                // Ainda não chegou à posição de patrulha? Move-se para a próxima "casa"
                mob->position = get_next_patrol_path_position(mob->position, mob->targetPosition);
            }
        } else {
            // Behaviour de atacar player / perseguição
            Node *nodes = map_to_node_system(map, r, c);
            //find_path(&nodes[mob->position.y * c + mob->position.x], &nodes[player->position.y * c + player->position.x]);
        }
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

// Algoritmo de Bresenham, verifica a visibilidade entre a posição do jogador e a posição do inimigo
int can_see_location(Vector2D posA, Vector2D posB, int distance, MAP** map){
    int dx = abs(posB.x - posA.x);
    int dy = abs(posB.y - posA.y);
    int sx = (posA.x < posB.x) ? 1 : -1;
    int sy = (posA.y < posB.y) ? 1 : -1;
    int err = dx - dy;
     
    Vector2D pos = {posA.x, posA.y};
    
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

        distance--;
    }

    if(pos.x == posB.x && pos.y == posB.y){
        return 1;
    }
    return 0;
}

Vector2D pick_random_patrol_position(Vector2D pos, MAP **map){
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

int available_floor_in_direction(Vector2D pos, MAP **map, Direction direction){
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