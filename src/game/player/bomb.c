#include <ncurses.h>
#include "bomb.h"
#include "engine_types.h"
#include "game_types.h"
#include "mobs_ai.h"
#include "image.h"
#include "draw.h"
#include "stdlib.h"
#include "sound.h"
#include <pthread.h>
#include "player.h"

/*

* a104356 - João Lobo

* Causa uma explosão numa dada área, onde o dano aplicado varia consoante a distância das entidades à posição da explosão.

*/
void explode_at_pos(Vector2D pos, int damage, World *world, GameState *state, int radius){
    // Aplicar dano a mobs
    for(int i = 0; i < world->mobQuantity; i++){
        int dist = distance_between_points(world->mobs[i].position, pos);
        if(dist <= radius){
            // Está na area de explosão (um circulo de raio radius)
            int actualDamage = (radius - dist) * damage;
            apply_damage_to_enemy(i, world, actualDamage);
        }
    }

    //Aplicar dano ao jogador
    int dist = distance_between_points(state->player.position, pos);
    if(dist <= radius){
        int actualDamage = (radius - dist) * damage;
        state->player.health -= actualDamage;
    }

    // Som da explosão
    pthread_t thread1;
    Sound *sound = malloc(sizeof(Sound));
    sound->filename = "assets/sound/explosion.wav";
    sound->time_ms = 1000;
    sound->loop = 0;
    if (pthread_create(&thread1, NULL, play_sound_thread, sound) != 0)  printw("Error creating thread\n");
}   

/*

* a104356 - João Lobo

* Desenha um frame da animação de explosão.

*/
void play_explosion_animation(Vector2D pos, int frame){
    frame = 3 - frame; 
    char location[100];
    pos.x -= 12;
    pos.y -= 12;
    sprintf(location, "assets/sprites/animations/explosion/%d.sprite", frame);
    Image frameImage = load_image_from_file(location);
    draw_to_screen(frameImage, pos);
}

/*

* a104356 - João Lobo

* Elimina uma bomba do mundo e limpa a memória correspondente.

*/
void destroy_bomb(World *world, int bombIndex){
    if(world->bombQuantity > 1){
        world->bombs[bombIndex] = world->bombs[world->bombQuantity - 1];
        world->bombs = realloc(world->bombs, sizeof(Bomb) * (world->bombQuantity - 1));
    }
    world->bombQuantity--;
}

/*

* a104356 - João Lobo

* Atualiza o temporizador das bombas do mapa e decide o comportamento da bomba (se deve explodir, desenhar as animações, desparecer, etc..).

*/
void update_bombs_timer(World *world, unsigned long elapsedMicroseconds, GameState *state){
    for(int i = 0; i < world->bombQuantity; i++){
        if(world->bombs[i].explosionFrame == -1){
            world->bombs[i].timeUntilExplosion -= elapsedMicroseconds;
        }
    }

    for(int i = 0; i < world->bombQuantity; i++){
        if(world->bombs[i].timeUntilExplosion < 0){
            if(world->bombs[i].explosionFrame == -1){
                explode_at_pos(world->bombs[i].position, world->bombs[i].damage, world, state, world->bombs[i].radius);
                world->bombs[i].explosionFrame = 3;
                world->bombs[i].timeUntilExplosion = -40000;
            } else {
                if(world->bombs[i].explosionFrame == 0){
                    clear();
                    destroy_bomb(world, i);
                } else {
                    world->bombs[i].timeUntilExplosion += elapsedMicroseconds;
                    if(world->bombs[i].timeUntilExplosion >= 0){
                        play_explosion_animation(world->bombs[i].position, world->bombs[i].explosionFrame);
                        world->bombs[i].timeUntilExplosion = -40000;
                        world->bombs[i].explosionFrame--;
                    }
                }
            }
            // Evitar explodir outras bombas neste update para não causar erros de memória
            return;
        }
    }
}

/*

* a104356 - João Lobo

* Adiciona uma bomba ao mapa. Recebe como argumentos as características da bomba.

*/
void place_bomb(Vector2D pos, int damage, int radius, World *world, unsigned long time){
    Bomb bomb;
    bomb.damage = damage;
    bomb.position = pos;
    bomb.timeUntilExplosion = time;
    bomb.radius = radius;
    bomb.explosionFrame = -1;

    world->bombs = realloc(world->bombs, sizeof(Bomb) * (world->bombQuantity + 1));
    world->bombs[world->bombQuantity] = bomb;
    world->bombQuantity++;

    if(time == 5000000){
        // Som do timer da bomba
        pthread_t thread1;
        Sound *sound = malloc(sizeof(Sound));
        sound->filename = "assets/sound/bomb_5s_timer.wav";
        sound->time_ms = 5000;
        sound->loop = 0;
        if (pthread_create(&thread1, NULL, play_sound_thread, sound) != 0)  printw("Error creating thread\n");
    }
}

/*

* a104356 - João Lobo

* Desenha bomba ou frame de explosão (consoante o estado da bomba).

*/
void draw_bomb(Bomb *bomb, Terminal *terminal){
    if(bomb->explosionFrame == -1){
        draw_custom_pixel(bomb->position, "||", 44, 0, terminal);
    } else {
        play_explosion_animation(bomb->position, bomb->explosionFrame);
    }
}