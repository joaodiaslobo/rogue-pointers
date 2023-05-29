#include "game.h"
#include <ncurses.h>
#include <stdlib.h>
#include "map.h"
#include "time.h"
#include "game_types.h"
#include "engine_types.h"
#include "player.h"
#include "inventory_menu.h"
#include "image.h"
#include "draw.h"
#include "mobs_ai.h"
#include <unistd.h>
#include "sys/time.h"
#include "components.h"
#include "sound.h"
#include "global_items.h"
#include "player_pathfinding.h"
#include <pthread.h>
#include "math.h"
#include "player_info.h"
#include "enemy_info.h"
#include "bullet.h"
#include "pop_up_ui.h"
#include "bomb.h"
#include "beacon.h"
#include "cheat_console.h"

int LEVEL, num_levels = 20;

/*

* a104356 - João Lobo

* Cria o estado inicial do jogo.

*/
GameState *init_game_state(){
	GameState *state = malloc(sizeof(GameState));
	if (state == NULL) {
        fprintf(stderr, "ERRO: Nao foi possivel alocar memoria\n");
        exit(EXIT_FAILURE);
    }
	Vector2D pos = {20,20};
	char name[15] = "NAME";
	state->player = *init_player(name, pos);
	state->gameOver = 0;
	state->paused = 0;
	state->pathSelection = 0;
	state->timeSinceLastBulletUpdate = 50000;
	PathBehaviour pathBehaviour = {pos, NULL, 0, 0, 0};
	state->pathState = pathBehaviour;
	state->mobsInUI = 0;
	return state;
}

/*

* a104179, a104439, a104356 - Sara Lopes, Rita Camacho, João Lobo

* Recebe inputs do utilizador e executa acções correspondentes (se existirem).

*/
void execute_input(GameState *state, World *w, int r, int c, Terminal *terminal){
	int key = getch();
    
	switch (key)
	{
		case KEY_UP:
		case '8':
			if(state->pathSelection == 0){
				apply_movement(state, NORTH, w[LEVEL].map, r, c);
			} else {
				apply_path_change(state, NORTH, w[LEVEL].map, r, c);
			}
			break;
		case KEY_DOWN:
		case '2':
			if(state->pathSelection == 0){
				apply_movement(state, SOUTH, w[LEVEL].map, r, c);
			} else {
				apply_path_change(state, SOUTH, w[LEVEL].map, r, c);
			}
			break;
		case KEY_RIGHT:
		case '6':
			if(state->pathSelection == 0){
				apply_movement(state, EAST, w[LEVEL].map, r, c);
			} else {
				apply_path_change(state, EAST, w[LEVEL].map, r, c);
			}
			break;
		case KEY_LEFT:
		case '4':
			if(state->pathSelection == 0){
				apply_movement(state, WEST, w[LEVEL].map, r, c);
			} else {
				apply_path_change(state, WEST, w[LEVEL].map, r, c);
			}
			break;
		case KEY_A1:
		case '7':
			apply_movement(state, NORTH, w[LEVEL].map, r, c);
			apply_movement(state, WEST, w[LEVEL].map, r, c);
			break;
		case KEY_A3:
		case '9':
			apply_movement(state, NORTH, w[LEVEL].map, r, c);
			apply_movement(state, EAST, w[LEVEL].map, r, c);
			break;
		case KEY_C1:
		case '1':
			apply_movement(state, SOUTH, w[LEVEL].map, r, c);
			apply_movement(state, WEST, w[LEVEL].map, r, c);
			break;
		case KEY_C3:
		case '3':
			apply_movement(state, SOUTH, w[LEVEL].map, r, c);
			apply_movement(state, EAST, w[LEVEL].map, r, c);
			break;
		case 10:
			// Ações do jogador
			perform_action(state, &w[LEVEL]);
			break;
		case 'q':
			endwin();
			exit(0);
			break;
		case 'u':
			check_for_portal(state, w, r, c, -1);
			break;
		case 'd':
			check_for_portal(state, w, r, c, 1);
			break;
		case '+':
			{
				char *input = malloc(sizeof(char) * 40);
				Vector2D inputPos = {terminal->xMax / 2 - 15, terminal->yMax / 2};
				text_input_box(inputPos, 45, 39, "Run command:", input);
				clear();
				refresh();
				execute_command(input, state, w[LEVEL].map);
				free(input);
			}
			break;
		case 'o':
			if(w[LEVEL].map[state->player.position.y][state->player.position.x].object == 9 && w[LEVEL].collectedChestItems == 0){
				open_chest(&(state->player.inventory));
				if(LEVEL % 5 == 0){
					new_bomb(&(state->player.inventory));
				}
				if (LEVEL % 2 == 1){
					// De 2 em 2 níveis o jogador recebe um beacon
					Item beacon = globalItems[12];
					add_item(&(state->player.inventory), &beacon);
				}
				if(LEVEL % 11 == 0){
					Item glowstick = globalItems[11];
					add_item(&(state->player.inventory), &glowstick);
				}
				w[LEVEL].collectedChestItems = 1;
				new_items_pop_up("New item(s) unlocked!", 35, terminal->yMax, terminal->xMax);
			}
			break;
		case KEY_MOUSE:
			{
				MEVENT event;
				if(getmouse(&event) == OK){
					if(event.bstate & BUTTON1_CLICKED){
						int buttonToolbarX = (terminal->xMax / 2) - (44 / 2);
						Vector2D buttonPausePos = {buttonToolbarX+10+4+15+4,terminal->yMax-1};
						Vector2D buttonInvPos = {buttonToolbarX+10+4,terminal->yMax-1};
						Vector2D buttonHelpPos = {buttonToolbarX,terminal->yMax-1};
						if((event.x >= buttonInvPos.x && event.x <= 16 + buttonInvPos.x) && event.y == buttonInvPos.y){
							state->paused = 1;
							WINDOW * inventoryWindow = newwin(terminal->yMax, terminal->xMax, 0, 0);
							box(inventoryWindow, 0, 0); 
							clear();
							refresh();
							show_inventory(terminal, inventoryWindow, state);
							wrefresh(inventoryWindow);
							delwin(inventoryWindow);
							clear();
							refresh();
							state->paused = 0;
						} else if((event.x >= buttonPausePos.x && event.x <= 11 + buttonPausePos.x) && event.y == buttonPausePos.y){
							pause_pop_up("GAME PAUSED", 45, terminal->yMax, terminal->xMax, terminal);
						} else if((event.x >= buttonHelpPos.x && event.x <= 10 + buttonHelpPos.x) && event.y == buttonHelpPos.y){
							help_pop_up(terminal);
						}
						else {
							Vector2D clickPos = {event.x / 2, event.y};
							apply_mouse_path_selection(state, w[LEVEL].map, clickPos, r, c);
						}
					} else if(event.bstate & BUTTON3_CLICKED){
						// Para armas ao longe
						if(state->player.inventory.items[state->player.selectedSlot].type == RANGED_WEAPON){
							// Cooldown
							if(state->player.timeSinceLastAction >= state->player.inventory.items[state->player.selectedSlot].cooldown){
								Vector2D clickPos = {event.x / 2, event.y};
								shoot_bullet(state->player.position, clickPos, state->player.inventory.items[state->player.selectedSlot].damage, &w[LEVEL]);
								state->player.timeSinceLastAction = 0;
							}
						}
					}
				}
			}
			break;
		case 'm':
			if(state->pathSelection && !state->pathState.moving){
				if(state->pathState.pathStep != 0){
					state->pathState.moving = 1;
				} else {
					state->pathSelection = 0;
				}
			} else{
				state->pathSelection = 1;
				state->pathState.pathPos = state->player.position;
			} 
			break;
		case 32: // Suspende o jogo
			pause_pop_up("GAME PAUSED", 45, terminal->yMax, terminal->xMax, terminal);
			break;
		default:
			break;
	}
}

/*

 * a104179 - Sara Lopes

 * Após ser detetado que o jogador quer mudar de nível, gera um novo mapa do mundo mais profundo ou navega para um já existente. 

*/
void check_for_portal(GameState *state, World *w, int r, int c, int dir){
	if (w[LEVEL].map[state->player.position.y][state->player.position.x].object == 2) { // encontrou um porta, muda de nível e gera um novo mapa
		if (dir == -1 && LEVEL > 0) {
			LEVEL--;
		}
		if (dir == 1 && LEVEL < num_levels - 1) {
			LEVEL++;
		} 
		if (w[LEVEL].created == 0) {
			gen_map(w[LEVEL].map,r,c);
			gen_lava(w[LEVEL].map,r,c);
			gen_grass(w[LEVEL].map,r,c);
			gen_water(w[LEVEL].map,r,c);
			w[LEVEL].mobQuantity = gen_mobs(&w[LEVEL].mobs, w[LEVEL].map, r,c, LEVEL);
			w[LEVEL].created = 1;
		}
		clear();
		while (w[LEVEL].map[state->player.position.y][state->player.position.x].object != 0 && (!(w[LEVEL].map[state->player.position.y][state->player.position.x].object == 2 && (LEVEL == 0 || LEVEL == num_levels-1)))) {
			state->player.position.x = (random() % c);
			state->player.position.y = (random() % r);

		}
		if (LEVEL == num_levels - 1) {
			state->gameOver = 2; // ganhou o jogo
		}
	} 
}

/*

* a104356 - João Lobo

* Atualiza todas as entidades e elementos do mapa consoante o tempo passado desde o último frame.

*/
void update(GameState *state, World *worlds, int r, int c, struct timeval currentTime, Terminal *terminal) {
	execute_input(state, worlds, r, c, terminal);
	
	for(int i = 0; i < worlds[LEVEL].mobQuantity; i++){
		switch (worlds[LEVEL].mobs[i].mobBehavior)
		{
		case STUPID:
			wander_ai(&worlds[LEVEL].mobs[i], &state->player, worlds[LEVEL].map, r, c);
			break;
		case INTELLIGENT:
			tactical_ai(&worlds[LEVEL].mobs[i], &state->player, worlds[LEVEL].map, &worlds[LEVEL]);
		default:
			break;
		}
	}

	struct timeval endTime;
	gettimeofday(&endTime, NULL);

	unsigned long elapsedMicroseconds = (endTime.tv_sec - currentTime.tv_sec) * 1000000 + (endTime.tv_usec - currentTime.tv_usec);
	for(int i = 0; i < worlds[LEVEL].mobQuantity; i++){
		update_timer(&worlds[LEVEL].mobs[i], elapsedMicroseconds);
	}

	if(state->pathSelection && state->pathState.moving){
		update_player_path(state, elapsedMicroseconds);
	}

	update_drowning(worlds[LEVEL].map, state, elapsedMicroseconds);

	if(worlds[LEVEL].bombQuantity > 0){
		update_bombs_timer(&worlds[LEVEL], elapsedMicroseconds, state);
	}

	if(state->player.timeSinceDrownStart > 10000000 || state->player.health <= 0){
		state->player.health = 0;
		state->gameOver = 1;
	}

	if(state->player.timeSinceLastAction < 10000000){
		state->player.timeSinceLastAction += elapsedMicroseconds;
	} else if(state->player.timeSinceLastAction > 10000000){
		state->player.timeSinceLastAction = 10000000;
	}

	if(state->timeSinceLastBulletUpdate < 50000){
		state->timeSinceLastBulletUpdate += elapsedMicroseconds;
	} else {
		for(int i = 0; i < worlds[LEVEL].bulletQuantity; i++){
			update_bullet(&worlds[LEVEL].bullets[i], worlds[LEVEL].map, i, &worlds[LEVEL], state);
		}
		state->timeSinceLastBulletUpdate = 0;
	}
	
	refresh();
}

/*

* a104179, a104439, a104356 - Sara Lopes, Rita Camacho, João Lobo

* Loop principal do programa, responsável por desenhar e atualizar o jogo.

*/
int game(Terminal *terminal, char *playerName) {
	clear();
	GameState *gameState = init_game_state();

	strcpy(gameState->player.name, playerName);

	int ncols, nrows;
    ncols = terminal->xMax;
	ncols = ncols/2;
    nrows = terminal->yMax;
    
	// Criação e inicialização do mapa 

	short buttonGradient[4] = {16,17,18,19};
	LEVEL = 0;
    World* worlds = (World*)malloc(num_levels * sizeof(World));
	if (worlds == NULL) {
		exit(EXIT_FAILURE);
	}
    for (int i = 0; i < num_levels; i++) {
		worlds[i].collectedChestItems = 0;
		worlds[i].created = 0;
		worlds[i].map = malloc(nrows * sizeof(Map*));
		worlds[i].mobQuantity = 0;
		worlds[i].bullets = malloc(sizeof(Bullet));
        worlds[i].bulletQuantity = 0;
        worlds[i].bombs = malloc(sizeof(Bomb));
		worlds[i].bombs[0].timeUntilExplosion = 1;
        worlds[i].bombQuantity = 0;
		worlds[i].beaconLocations = malloc(sizeof(Vector2D));
		worlds[i].beacons = 0;
	    if (worlds[i].map == NULL) {
		   exit(EXIT_FAILURE);
	   	}
	   for (int j = 0; j < nrows; j++) {
		   worlds[i].map[j] = (Map*)malloc(ncols * sizeof(Map));
		   if (worlds[i].map[j] == NULL) {
			   exit(EXIT_FAILURE);
		   }
		   for (int k = 0; k < ncols; k++) {
			   worlds[i].map[j][k].object = 3; // Inicializando o valor do campo object como 3 (vazio)
			   worlds[i].map[j][k].visited = 0;
		   }
	   }
	}

	srandom(time(NULL));

	intrflush(stdscr, false);
    
    // Gera e imprime o primeiro mapa/nível do mundo
	gen_map(worlds[LEVEL].map,nrows,ncols);
	worlds[LEVEL].created = 1;
	gen_grass(worlds[LEVEL].map,nrows,ncols); // No nível 0 é possível existir relva
	worlds[LEVEL].mobQuantity = 0;
	worlds[LEVEL].bulletQuantity = 0;
	print_map(worlds[LEVEL].map, nrows, ncols, gameState, terminal);

	// Inicializa a posição do jogador num lugar válido dentro do mapa
	while(worlds[LEVEL].map[gameState->player.position.y][gameState->player.position.x].object != 0 && worlds[LEVEL].map[gameState->player.position.y][gameState->player.position.x].object != 4 && worlds[LEVEL].map[gameState->player.position.y][gameState->player.position.x].object != 5) {
		gameState->player.position.x = (random() % ncols);
		gameState->player.position.y = (random() % nrows);
	}
	gameState->gameOver = 0;

	endwin(); 
    
	struct timeval currentTime;
	while(1) {
		if(!gameState->paused){
			gettimeofday(&currentTime, NULL);
			move(nrows - 1, 0);
			attron(COLOR_PAIR(COLOR_WHITE));
			printw("(%d, %d) %d %d", gameState->player.position.x, gameState->player.position.y, ncols, nrows);
			attroff(COLOR_PAIR(COLOR_WHITE));

			if(gameState->pathSelection == 1){
				draw_path(gameState);
			}
			
			print_map(worlds[LEVEL].map, nrows, ncols, gameState, terminal);
			draw_mobs(worlds[LEVEL].mobs, worlds[LEVEL].mobQuantity, terminal);
			draw_custom_pixel(gameState->player.position, "<>", 35, 4, terminal);

			for(int i = 0; i < worlds[LEVEL].bulletQuantity; i++){
				draw_bullet(&worlds[LEVEL].bullets[i], terminal);
			}

			for(int i = 0; i < worlds[LEVEL].bombQuantity; i++){
				draw_bomb(&worlds[LEVEL].bombs[i], terminal);
			}

			for(int i = 0; i < worlds[LEVEL].beacons; i++){
				draw_beacon(worlds[LEVEL].beaconLocations[i], terminal);
			}

			if(!gameState->player.fullBright){
				draw_light(gameState, nrows, ncols, worlds[LEVEL].map, &worlds[LEVEL], terminal);
			}

			// Botões
			int buttonToolbarX = (terminal->xMax / 2) - (44 / 2);
			Vector2D buttonHelpPos = {buttonToolbarX,terminal->yMax-1};
			button(buttonGradient, "Help", buttonHelpPos);
			Vector2D buttonInvPos = {buttonToolbarX+10+4,terminal->yMax-1};
			button(buttonGradient, "Inventory", buttonInvPos);
			Vector2D buttonPausePos = {buttonToolbarX+10+4+15+4,terminal->yMax-1};
			button(buttonGradient, "Pause", buttonPausePos);

			if (gameState->gameOver == 1){
				for (int i = 0; i < num_levels; i++) {
 				    free(worlds[i].bullets);
			        for (int j = 0; j < nrows; j++) {
        				free(worlds[i].map[j]);
    				}
    				free(worlds[i].map);
				}
				free(worlds); 
				game_over_pop_up("GAME OVER :(", 45, terminal->yMax, terminal->xMax, terminal);
				return(0);
			}
			if (gameState->gameOver == 2){
				for (int i = 0; i < num_levels; i++) {
 				    free(worlds[i].bullets);
			        for (int j = 0; j < nrows; j++) {
        				free(worlds[i].map[j]);
    				}
    				free(worlds[i].map);
				}
				free(worlds);
				winner_pop_up("CONGRATS, YOU WON!", 50, terminal->yMax, terminal->xMax, terminal);
				return(0);
			}

			player_info_ui(gameState, terminal, LEVEL);

			Vector2D enemyInfoPos = { 0, 5 };
			enemy_info_ui(gameState, &worlds[LEVEL], enemyInfoPos, terminal);
		}

		update(gameState, worlds, nrows, ncols, currentTime, terminal);
	}

	return 0;
}