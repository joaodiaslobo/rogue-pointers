#include "player.h"
#include "game.h"
#include "game_types.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "image.h"
#include "sound.h"
#include <pthread.h>
#include "draw.h"
#include "inventory.h"
#include "global_items.h"
#include "mobs_ai.h"
#include "player_pathfinding.h"
#include <ncurses.h>

Player *init_player(char name[15], Vector2D pos){
    Player *player = malloc(sizeof(Player));
    if (player == NULL) {
        fprintf(stderr, "ERRO: Nao foi possivel alocar memoria\n");
        exit(EXIT_FAILURE);
    }

    Inventory inventory = initialize_inventory();

    strcpy(player->name, name);
    player->position = pos;
    player->health = 100;
    player->gold = 0;
    player->inventory = inventory;
    player->selectedSlot = 0;
    player->speedMultiplier = 1;
    player->timeSinceDrownStart = 0;
    player->timeSinceLastAction = 10000000;

    Item firstItem = globalItems[pick_random_item(&player->inventory)]; //para mesmo que não recolha itens consiga lutar no nível seguinte
    add_item(&player->inventory, &firstItem);

    Item rock = globalItems[16];
    add_item(&player->inventory, &rock);

    return player;
}

void perform_action(GameState *gameState, World* world){
    int actionType = gameState->player.inventory.items[gameState->player.selectedSlot].type; 

    switch (actionType)
    {
        case MELEE_WEAPON:
            if(gameState->player.timeSinceLastAction >= gameState->player.inventory.items[gameState->player.selectedSlot].cooldown){
                int attackDamage = gameState->player.inventory.items[gameState->player.selectedSlot].damage;

                for(int i = 0; i < world->mobQuantity; i++){
                    if(distance_between_points(gameState->player.position, world->mobs[i].position) <= 1.5f){
                        apply_damage_to_enemy(i, world, attackDamage);
                    }
                }

                gameState->player.timeSinceLastAction = 0;
            }
            break;
        default:
            break;
    }
}

void apply_damage_to_enemy(int enemyIndex, World *world, int damage){
    Mob *mob = &(world->mobs[enemyIndex]);

    mob->health -= damage;

    if(mob->health <= 0){
        remove_enemy_from_world(world, enemyIndex);
    }
}

void apply_movement(GameState *gameState, Direction facing, Map** map, int r, int c){
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

    // Valida se o jogador pisou a lava, se tem pedras no inventário continua, caso contrário morre
    if(map[newPos.y][newPos.x].object == 4){
        int rock = 0;
        rock = get_item_quantity_by_type(&gameState->player.inventory, MISCELLANEOUS);
        if(rock == 1){ //tranforma lava em caminho
            map[newPos.y][newPos.x].object = 0;
            gameState->player.position.x = newPos.x;
            gameState->player.position.y = newPos.y;    
        }else{ //morre queimado pela lava
            gameState->player.position.x = newPos.x;
            gameState->player.position.y = newPos.y;
            pthread_t thread1; // Cria uma thread para reproduzir o som do jogador a cair na lava
            Sound *fich1 = malloc(sizeof(Sound));
            fich1->filename = "assets/sound/player_burnt.wav";
            fich1->time_ms = 1000;
            fich1->loop = 0;
            if (pthread_create(&thread1, NULL, play_sound_thread, fich1) != 0)  printw("Error creating thread\n");
            gameState->gameOver = 1;
        }
    }

    // Se o jogador encountrou a chave guarda no inventário
    if(map[newPos.y][newPos.x].object == 11){
        gameState->player.position.x = newPos.x;
        gameState->player.position.y = newPos.y;

        Item key = globalItems[15];
        add_item(&gameState->player.inventory, &key);
        map[newPos.y][newPos.x].object = 0; //apaga a chave do mapa
        pthread_t thread1; // Cria uma thread para reproduzir o som da chave a ser apanhada
        Sound *fich1 = malloc(sizeof(Sound));
        fich1->filename = "assets/sound/pick.wav";
        fich1->time_ms = 1000;
        fich1->loop = 0;
        if (pthread_create(&thread1, NULL, play_sound_thread, fich1) != 0)  printw("Error creating thread\n");
    }

    // Se o jogador encontrou a porta e tem uma chave, a porta abre e ouve-se o som
    if(map[newPos.y][newPos.x].object == 10){
        int key_qty = 0;
        key_qty = get_key_quantity(&gameState->player.inventory);
        if(key_qty == 1){
            for(int i = 1; i < r; i++) {  
		        for(int j = 1; j < c; j++) {
                    if(map[i][j].object == 10) map[i][j].object = 0;
                }
            }
            pthread_t thread1; // Cria uma thread para reproduzir o som da porta a abrir
            Sound *fich1 = malloc(sizeof(Sound));
            fich1->filename = "assets/sound/door_opening.wav";
            fich1->time_ms = 1000;
            fich1->loop = 0;
            if (pthread_create(&thread1, NULL, play_sound_thread, fich1) != 0) printw("Error creating thread\n");
        }
        delete_key(&gameState->player.inventory);
    }

    // Se o jogador encontrou uma arca guarda pedra no inventário
    if(map[newPos.y][newPos.x].object == 9){
        gameState->player.inventory.items[0].picked += 15;
            pthread_t thread1; // Cria uma thread para reproduzir o som de apanhar objeto
            Sound *fich1 = malloc(sizeof(Sound));
            fich1->filename = "assets/sound/pick.wav";
            fich1->time_ms = 1000;
            fich1->loop = 0;
            if (pthread_create(&thread1, NULL, play_sound_thread, fich1) != 0) printw("Error creating thread\n");
    }

    // Som para o jogador a nadar na água
    if(map[newPos.y][newPos.x].object == 7){
        pthread_t thread1; // Cria uma thread para reproduzir o som do jogador a cair na lava
        Sound *fich1 = malloc(sizeof(Sound));
        fich1->filename = "assets/sound/water_dive.wav";
        fich1->time_ms = 1500;
        fich1->loop = 0;
        if (pthread_create(&thread1, NULL, play_sound_thread, fich1) != 0)  printw("Error creating thread\n");
    }

}

void update_drowning(Map **map, GameState *gameState, unsigned long elapsedMicroseconds){
    if(map[gameState->player.position.y][gameState->player.position.x].object == 7){
        gameState->player.timeSinceDrownStart += elapsedMicroseconds;
    } else {
        gameState->player.timeSinceDrownStart = 0;
    }
}

void draw_light(GameState *gameState, int r, int c, Map **map, Terminal *terminal){
    Vector2D pos;
    Image image = load_image_from_file("assets/sprites/shadow.sprite");
    for(int i = 0; i < c; i++){
        for(int j = 0; j < r-1; j++){
            pos.x = i;
            pos.y = j;
            //equação de um círculo -> (x-a)² + (y-b)² <= raio², sendo (a,b) a posição do jogador, e verificação se antes dessa posição, na mesma diagonal, há parede - caso haja, fica às escuras a partir daí na diagonal            
            if(map[j][i].object != 3 && !(gameState->pathSelection == 1 && is_cell_path_part(gameState, pos)) && ((i - (gameState->player.position.x))*(i - (gameState->player.position.x)) + ((j - (gameState->player.position.y))*(j - (gameState->player.position.y))) > 4096 || !(light_before_walls(pos, gameState->player.position, 64, map)))){
                if(map[j][i].visited == 0){
                    draw_to_screen(image, pos);
                }
                else if(map[j][i].visited == 1){
                    if(map[j][i].object == 1){
				        draw_custom_pixel(pos, "##", 85, choose_color(71, 2, pos), terminal);
                    }
                    else if(map[j][i].object == 4){
                        draw_empty_pixel(pos, 87);
                    }
                    else if(map[j][i].object == 7){
                        draw_custom_pixel(pos, "~ ", 75, 76, terminal);
                    }
                    else if(map[j][i].object == 8){
                        draw_custom_pixel(pos, " .", 76, 75, terminal);
                    }
                    else{
                        draw_custom_pixel(pos, ". ", 71, 86, terminal);
                    }
                }
            }
            else if(map[j][i].object != 3){
                map[j][i].visited = 1;
            }
        }
    }
}

int light_before_walls(Vector2D posA, Vector2D posB, int distance, Map** map){
    int dx = abs(posB.x - posA.x);
    int dy = abs(posB.y - posA.y);
    int sx, sy;

    if(posA.x < posB.x){
        sx = 1;
    } else {
        sx = -1;
    }

    if(posA.y < posB.y){
        sy = 1;
    } else {
        sy = -1;
    }

    int err = dx - dy;
     
    Vector2D pos = {posA.x, posA.y};
    
    do {

        int e2 = 2 * err;
        if (e2 > -dy){

            err -= dy;
            pos.x += sx;
        }
        if (e2 < dx){

            err += dx;
            pos.y += sy;
        }

        distance--;

    } while((pos.x != posB.x || pos.y != posB.y) && (map[pos.y][pos.x].object != 1) && (map[pos.y][pos.x].object != 3) && (distance > 0));

    if(pos.x == posB.x && pos.y == posB.y){
        return 1;
    }
    return 0;
}

void open_chest(Inventory *inventory){

    int new = pick_random_item(inventory);
    if(new != -1){
        Item newItem = globalItems[new];
        add_item(inventory, &newItem);
        mvprintw(11,0, "New item(s) unlocked!");
    }
}

void new_bomb(Inventory *inventory){
    Item newBomb = globalItems[10 + (LEVEL / 5)];
    add_item(inventory, &newBomb);
    globalItems[10 + (LEVEL / 5)].picked = 1 + choose_item_freq(BOMB);
}