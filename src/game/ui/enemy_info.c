#include "enemy_info.h"
#include "game_types.h"
#include "mobs_ai.h"
#include "components.h"
#include "draw.h"

void enemy_info_ui(GameState *state, World *world, Vector2D pos){
    int newAmountMobsUI = 0;
    for(int i = 0; i < world->mobQuantity; i++){
        // Mostra apenas inimigos próximos
        if(distance_between_points(state->player.position, world->mobs[i].position) < 5){
            Vector2D healthPos = { pos.x , pos.y + newAmountMobsUI * 3 + 1};
            switch (world->mobs[i].mobBehavior)
            {
            case STUPID:
                mvprintw(pos.y + newAmountMobsUI * 3, pos.x, "Mindless Zombie");
                progress_bar(world->mobs[i].health, 100, 20, 20, 21, "Health", healthPos);
                break;
            case INTELLIGENT:
                mvprintw(pos.y + newAmountMobsUI * 3, pos.x, "Tactical Zombie");
                progress_bar(world->mobs[i].health, 50, 20, 20, 21, "Health", healthPos);
                break;
            default:
                break;
            }
            newAmountMobsUI++;
        }
    }

    if(newAmountMobsUI != state->mobsInUI){
        state->mobsInUI = newAmountMobsUI;
        clean_ui();
    }
}

void clean_ui(){
    for(int x = 0; x < 10; x++){
        for(int y = 4; y < 100; y++){
            Vector2D pos = { x, y };
            draw_empty_pixel(pos, -8);
        }
    }
}