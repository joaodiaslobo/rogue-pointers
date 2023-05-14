#include "player.h"
#include "game.h"
#include "game_types.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "image.h"
#include "sound.h"

Player *init_player(char name[15], Vector2D pos){
    Player *player = malloc(sizeof(Player));
    if (player == NULL) {
        fprintf(stderr, "ERRO: Nao foi possivel alocar memoria\n");
        exit(EXIT_FAILURE);
    }
    Inventory inventory;
    strcpy(player->name, name);
    player->position = pos;
    player->health = 50;
    player->gold = 0;
    player->inventory = inventory;
    player->selectedSlot = 0;
    player->speedMultiplier = 1;
    player->timeSinceDrownStart = 0;
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

    // Valida a posição do jogador no mapa, se este move para uma parede, porta ou para fora do mapa não pode avançar
    if(!(map[newPos.y][newPos.x].object == 1 ||  map[newPos.y][newPos.x].object == 10 || map[newPos.y][newPos.x].object == 3 || newPos.x < 0 || newPos.y < 0 || newPos.y >= r || newPos.x >= c)){
        gameState->player.position.x = newPos.x;
        gameState->player.position.y = newPos.y;
    }

    // Valida se o jogador pisou a lava e morreu
    if(map[newPos.y][newPos.x].object == 4){
        gameState->player.position.x = newPos.x;
        gameState->player.position.y = newPos.y;
        gameState->gameover = 1;
    }

    // Se o jogador encountrou a chave abre a porta da sala com o baú
    if(map[newPos.y][newPos.x].object == 11){
        gameState->player.position.x = newPos.x;
        gameState->player.position.y = newPos.y;
        for(int i = 1; i < r; i++) {  
		    for(int j = 1; j < c; j++) {
                if(map[i][j].object == 10){
                    map[i][j].object = 0;
                    play_sound(0);
                }
            }
        }       
    }

}

void update_drowning(MAP **map, GameState *gameState, unsigned long elapsedMicroseconds){
    if(map[gameState->player.position.y][gameState->player.position.x].object == 7){
        gameState->player.timeSinceDrownStart += elapsedMicroseconds;
    } else {
        gameState->player.timeSinceDrownStart = 0;
    }
}

void draw_light(GameState *gameState, int r, int c, MAP **map){
    Vector2D pos;
    Image image = load_image_from_file("assets/sprites/shadow.sprite");
    for(int i = 0; i < c; i++){
        for(int j = 0; j < r-1; j++){
            pos.x = i;
            pos.y = j;
            //equação de um círculo -> (x-a)² + (y-b)² <= raio², sendo (a,b) a posição do jogador
            if((i - (gameState->player.position.x))*(i - (gameState->player.position.x)) + ((j - (gameState->player.position.y))*(j - (gameState->player.position.y))) > 256 && map[j][i].object != 3){
                draw_to_screen(image, pos);
            }
        }
    }
}