#include "engine_types.h"
#include "game_types.h"
#include "player_pathfinding.h"
#include "a_star_pathfinding.h"
#include "draw.h"

/*

* a104356 - João Lobo

* Move o "cursor" que traceja o caminho do pathfinding do jogador.

*/
void apply_path_change(GameState *gameState, Direction facing, Map** map, int r, int c){
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

/*

* a104356 - João Lobo

* Cálcula um caminho do jogador até à posição onde o mouse clicou.

*/
void apply_mouse_path_selection(GameState *gameState, Map **map, Vector2D clickPos, int r, int c){
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

/*

* a104356 - João Lobo

* Desenha o caminho do pathfinding do jogador.

*/
void draw_path(GameState *gameState){
    if(gameState->pathState.pathStep == 0 || gameState->pathState.path == NULL){
        return;
    }

    int j = gameState->pathState.pathStep;

    if(gameState->pathState.moving){
        j++;
    }

    for(int i = 0; i < j; i++){
        draw_empty_pixel(gameState->pathState.path[i], 26);
    }
}

/*

* a104356 - João Lobo

* Verifica se uma coordenada faz parte do caminho do jogador (usado para evitar bugs visuais de flickering com as sombras).

*/
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

/*

* a104356 - João Lobo

* Atualiza a posição do jogador para a próxima no caminho do pathfinding tendo em conta o tempo que passou desde o último movimento.

*/
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