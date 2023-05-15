#include "engine_types.h"
#include "game_types.h"
#include "player_pathfinding.h"
#include "a_star_pathfinding.h"
#include "image.h"
#include "draw.h"

void apply_path_change(GameState *gameState, Direction facing, MAP** map, int r, int c){

    if(gameState->pathState.moving){
        return;
    }

    Vector2D newPos = {gameState->pathState.pathPos.x, gameState->pathState.pathPos.y};
    switch (facing)
    {
        case NORTH:
            newPos.y -= 1;
            break;
        case SOUTH:
            newPos.y += 1;
            break;
        case EAST:
            newPos.x += 1;
            break;
        case WEST:
            newPos.x -= 1;
            break;
        default:
            break;
    }

    // Valida a posição do jogador no mapa, se este move para uma parede, porta ou para fora do mapa não pode avançar
    if(!(map[newPos.y][newPos.x].object == 1 ||  map[newPos.y][newPos.x].object == 10 || map[newPos.y][newPos.x].object == 3 || newPos.x < 0 || newPos.y < 0 || newPos.y >= r || newPos.x >= c)){
        gameState->pathState.pathPos.x = newPos.x;
        gameState->pathState.pathPos.y = newPos.y;
        Node *nodes = map_to_node_system(map, r, c);
        int steps;
        gameState->pathState.path = find_path(nodes, gameState->pathState.pathPos, gameState->player.position, c, r, &steps);
        if(gameState->pathState.path != NULL){
            gameState->pathState.pathStep = steps - 1;
        }
    }
}

void apply_mouse_path_selection(GameState *gameState, MAP **map, Vector2D clickPos, int r, int c){
    gameState->pathSelection = 1;
    gameState->pathState.pathPos = gameState->player.position;
    
    if(clickPos.x > 0 && clickPos.y > 0 && clickPos.x < c && clickPos.y < r && !(map[clickPos.y][clickPos.x].object == 1 ||  map[clickPos.y][clickPos.x].object == 10 || map[clickPos.y][clickPos.x].object == 3)){
        gameState->pathState.pathPos.x = clickPos.x;
        gameState->pathState.pathPos.y = clickPos.y;
        Node *nodes = map_to_node_system(map, r, c);
        int steps;
        gameState->pathState.path = find_path(nodes, gameState->pathState.pathPos, gameState->player.position, c, r, &steps);
        if(gameState->pathState.path != NULL){
            gameState->pathState.pathStep = steps - 1;
        }
        gameState->pathState.moving = 1;
    }
}

void draw_path(GameState *gameState){
    if(gameState->pathState.pathStep == 0 || gameState->pathState.path == NULL){
        return;
    }

    Image pathOverlay = load_image_from_file("assets/sprites/miscellaneous/pathOverlay.sprite");

    int j = gameState->pathState.pathStep;

    if(gameState->pathState.moving){
        j++;
    }

    for(int i = 0; i < j; i++){
        draw_to_screen(pathOverlay, gameState->pathState.path[i]);
    }
}

int is_cell_path_part(GameState *gameState, Vector2D pos){
    if(gameState->pathState.pathStep == 0 || gameState->pathState.path == NULL){
        return 0;
    }

    for(int i = 0; i < gameState->pathState.pathStep; i++){
        if(gameState->pathState.path[i].x == pos.x && gameState->pathState.path[i].y == pos.y){
            return 1;
        }
    }

    return 0;
}

void update_player_path(GameState *gameState, unsigned long elapsedMicroseconds){
    gameState->pathState.timeSinceLastUpdate += elapsedMicroseconds;
    if(gameState->pathState.timeSinceLastUpdate > 250000){
        gameState->pathState.timeSinceLastUpdate = 0;
        if(gameState->pathState.moving && gameState->pathState.path != NULL  && gameState->pathState.pathStep >= 0){
            gameState->player.position = gameState->pathState.path[gameState->pathState.pathStep];
            gameState->pathState.pathStep--;
            if(gameState->pathState.pathStep < 0){
                PathBehaviour pathBehaviour = {gameState->player.position, NULL, 0, 0, 0};
                gameState->pathState = pathBehaviour;
                gameState->pathSelection = 0;
            }
        } else {
            PathBehaviour pathBehaviour = {gameState->player.position, NULL, 0, 0, 0};
            gameState->pathState = pathBehaviour;
            gameState->pathSelection = 0;
        }
    }
}