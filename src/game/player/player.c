#include "player.h"
#include "game.h"
#include "game_types.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "image.h"

struct timeval start_time_drown = (struct timeval) {0};
int elapsed_time_drown = 0;


Player *init_player(char name[15], Vector2D pos){
    Player *player = malloc(sizeof(Player));
    if (player == NULL) {
        fprintf(stderr, "ERRO: Nao foi possivel alocar memoria\n");
        exit(EXIT_FAILURE);
    }
    Inventory inventory;
    strcpy(player->name, name);
    player->position = pos;
    player->gold = 0;
    player->inventory = inventory;
    player->selectedSlot = 0;
    player->speedMultiplier = 1;
    return player;
}

void apply_movement(GameState *gameState, Direction facing, MAP** map, int r, int c){
    Vector2D newPos = {gameState->player.position.x, gameState->player.position.y};
    switch (facing)
    {
        case NORTH:
            newPos.y -= gameState->player.speedMultiplier;
            break;
        case SOUTH:
            newPos.y += gameState->player.speedMultiplier;
                break;
        case EAST:
            newPos.x += gameState->player.speedMultiplier;
            break;
        case WEST:
            newPos.x -= gameState->player.speedMultiplier;
                break;
        default:
            break;
    }

    // Valida a posição do jogador no mapa, se este move para uma parede ou para fora do mapa não pode avançar.
    if(!(map[newPos.y][newPos.x].object == 1 ||  map[newPos.y][newPos.x].object == 3 || newPos.x < 0 || newPos.y < 0 || newPos.y >= r || newPos.x >= c)){
        gameState->player.position.x = newPos.x;
        gameState->player.position.y = newPos.y;
    }

    // Valida se o jogador pisou a lava e morreu
    if(map[newPos.y][newPos.x].object == 4){
        gameState->player.position.x = newPos.x;
        gameState->player.position.y = newPos.y;
        gameState->gameover = 1;
    }

    // Inicializa a contagem do tempo, para verificar se o jogador se afogou
    if(map[newPos.y][newPos.x].object == 7 && start_time_drown.tv_sec == 0){
        gettimeofday(&start_time_drown, NULL); // obter o tempo inicial
        gameState->player.position.x = newPos.x;
        gameState->player.position.y = newPos.y;
    }
    else if (map[newPos.y][newPos.x].object == 7 && start_time_drown.tv_sec != 0){
        gameState->player.position.x = newPos.x;
        gameState->player.position.y = newPos.y;
    }
    if(map[newPos.y][newPos.x].object != 7 && map[newPos.y][newPos.x].object != 1 && start_time_drown.tv_sec != 0) start_time_drown = (struct timeval) {0};

    // Se o jogador encountrou a chave abre a porta da sala com o baú
    if(map[newPos.y][newPos.x].object == 11){
        gameState->player.position.x = newPos.x;
        gameState->player.position.y = newPos.y;
        for(int i = 1; i < r; i++) {  
		    for(int j = 1; j < c; j++) {
                if(map[i][j].object == 10) map[i][j].object = 0;
            }
        }       
    }

}

void draw_light(GameState *gameState, int r, int c){
    Vector2D pos;
    Image image = load_image_from_file("assets/sprites/shadow.sprite");
    for(int i = 0; i < c; i++){
        for(int j = 0; j < r-1; j++){
            pos.x = i;
            pos.y = j;
            //equação de um círculo -> (x-a)² + (y-b)² <= raio², sendo (a,b) a posição do jogador
            if((i - (gameState->player.position.x))*(i - (gameState->player.position.x)) + ((j - (gameState->player.position.y))*(j - (gameState->player.position.y))) > 256){
            //draw_to_screen(image, pos);
            }
        }
    }
}