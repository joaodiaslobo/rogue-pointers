#include "player.h"
#include "game.h"
#include "game_types.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern int NUM_COLUMNS;

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

void apply_movement(GameState *gameState, Direction facing, Map (*map)[NUM_COLUMNS], int r, int c){
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

    //printw("col:%d lin:%d tc:%d tl:%d Obs:%d col:%d, lin:%d", state->player.position.x, state->player.position.y, c, r, m[y][x].object, x,y);
}