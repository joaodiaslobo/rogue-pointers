#include "enemy_info.h"
#include "game_types.h"
#include "engine_types.h"
#include "mobs_ai.h"
#include "components.h"
#include "draw.h"

/*

* a104356 - João Lobo

* Desenha painéis com informação dos mobs caso estes estejam perto do jogador.

*/
void enemy_info_ui(GameState *state, World *world, Vector2D pos, Terminal *terminal){
    int newAmountMobsUI = 0;
    for(int i = 0; i < world->mobQuantity; i++){
        // Mostra apenas inimigos próximos
        if((distance_between_points(state->player.position, world->mobs[i].position) < 10) && (can_see_location(state->player.position, world->mobs[i].position, 10, world->map))){
            Vector2D healthPos = { pos.x , pos.y + newAmountMobsUI * 3 + 1};
            Vector2D mobDisplayPos = { pos.x , pos.y + newAmountMobsUI * 3};
            switch (world->mobs[i].mobBehavior)
            {
            case STUPID:
                mvprintw(pos.y + newAmountMobsUI * 3, pos.x + 4, "Mindless Zombie");
                progress_bar(world->mobs[i].health, 100, 20, 20, 21, "Health", healthPos);
                draw_custom_pixel(mobDisplayPos, "><", 35, 0, terminal);
                break;
            case INTELLIGENT:
                mvprintw(pos.y + newAmountMobsUI * 3, pos.x + 4, "Tactical Zombie");
                progress_bar(world->mobs[i].health, 50, 20, 20, 21, "Health", healthPos);
			    draw_custom_pixel(mobDisplayPos, "><", 35, 59, terminal);
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

/*

* a104356 - João Lobo

* Limpa painéis de informação dos mobs.

*/
void clean_ui(){
    for(int x = 0; x < 10; x++){
        for(int y = 4; y < 100; y++){
            Vector2D pos = { x, y };
            draw_empty_pixel(pos, -8);
        }
    }
}