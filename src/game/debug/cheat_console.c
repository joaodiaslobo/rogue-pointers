#include "game_types.h"
#include "inventory.h"
#include "global_items.h"
#include <string.h>

void execute_command(char *input, GameState *state){
    char command[20];
    int n;
    if (sscanf(input, "%19s %d", command, &n) == 2) {
        if (strcmp(command, "give_item") == 0) {
            Item item = globalItems[n];
            add_item(&state->player.inventory, &item);
        }
    } else if(strcmp(input, "restore_health") == 0) {
        state->player.health = 100;
    } else if(strcmp(input, "full_bright") == 0){
        state->player.fullBright = !state->player.fullBright;
    } else if(strcmp(input, "die") == 0){
        state->player.health = 0;
    }
}   