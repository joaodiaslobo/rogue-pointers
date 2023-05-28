#include "player_info.h"
#include "game_types.h"
#include "engine_types.h"
#include <math.h>
#include <string.h>
#include "draw.h"
#include "components.h"

void player_info_ui(GameState *gameState, Terminal *terminal, int level){
    //Display de status do jogador
    Vector2D playerDisplayPos = {0, 0};
    draw_custom_pixel(playerDisplayPos, "<>", 35, 4, terminal);
    mvprintw(0, 4, "You");

    Vector2D healthBarPos = {0,1};
    progress_bar(gameState->player.health, 100, 20, 20, 21, "Health", healthBarPos);

    int timeToDrownSecs = 10 - floor(gameState->player.timeSinceDrownStart * 0.000001);
    Vector2D oxygenBarPos = {0,2};
    progress_bar(timeToDrownSecs, 10, 20, 22, 23, "Oxygen", oxygenBarPos);
    

    // Display de item selecionado e cooldown se aplicável
    if(gameState->player.inventory.items[gameState->player.selectedSlot].type != NONE){
        mvprintw(3,0, "%s", gameState->player.inventory.items[gameState->player.selectedSlot].name);
        int cooldownIconPosX = strlen(gameState->player.inventory.items[gameState->player.selectedSlot].name) + 1;
        if(gameState->player.inventory.items[gameState->player.selectedSlot].type == MELEE_WEAPON){
            if(gameState->player.timeSinceLastAction < gameState->player.inventory.items[gameState->player.selectedSlot].cooldown){
                attron(COLOR_PAIR(8));
                mvaddch(3, cooldownIconPosX, ACS_PLUS);
                attroff(COLOR_PAIR(8));
            } else {
                mvaddch(3, cooldownIconPosX, ACS_PLUS);
            }
        } else if(gameState->player.inventory.items[gameState->player.selectedSlot].type == RANGED_WEAPON) {
            if(gameState->player.timeSinceLastAction < gameState->player.inventory.items[gameState->player.selectedSlot].cooldown){
                attron(COLOR_PAIR(8));
                mvaddch(3, cooldownIconPosX, ACS_ULCORNER);
                attroff(COLOR_PAIR(8));
            } else {
                mvaddch(3, cooldownIconPosX, ACS_ULCORNER);
            }
        } 
        else 
        {
            mvaddch(3, cooldownIconPosX, ' ');
        }
    } else {
        mvprintw(3,0, "                              ");
    }

    mvprintw(terminal->yMax - 1, 0, "Depth: %d", level);
}