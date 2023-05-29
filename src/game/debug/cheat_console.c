#include "game_types.h"
#include "inventory.h"
#include "global_items.h"
#include <string.h>

/*

* a104356 - João Lobo

* Recebe um comando da consola de desenvolvedor e executa a ação correspondente (se existir).

*/
void execute_command(char *input, GameState *state, Map **map){
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
    } else if(strcmp(input, "spawn_portal") == 0){
        map[state->player.position.y][state->player.position.x].object = 2;
    }
}   