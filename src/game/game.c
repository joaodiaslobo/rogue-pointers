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
#include "enemy_info.h"
#include "bullet.h"

int LEVEL, num_levels = 20;

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
		case 'o':
			if(w[LEVEL].map[state->player.position.y][state->player.position.x].object == 9 && w[LEVEL].collectedChestItems == 0){
				open_chest(&(state->player.inventory));
				if(LEVEL % 5 == 0){
					new_bomb(&(state->player.inventory));
					mvprintw(10, 0, "New bomb unlocked!");
				}
				w[LEVEL].collectedChestItems = 1;
			}
			break;
		case KEY_MOUSE:
			{
				MEVENT event;
				if(getmouse(&event) == OK){
					if(event.bstate & BUTTON1_CLICKED){
						int buttonToolbarX = (terminal->xMax / 2) - (73 / 2);
						Vector2D buttonInvPos = {buttonToolbarX+14+12+4+13+4+14,terminal->yMax-1};
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
			move(0,80);
            printw("Paused game. Press 'Space' to continue");
			refresh();
        	while (getch() != 32) {	
        	}
			clear();
			break;
		default:
			break;
	}
}

void check_for_portal(GameState *state, World *w, int r, int c, int dir){
	if (w[LEVEL].map[state->player.position.y][state->player.position.x].object == 2) { //encontrou um porta, muda de nível e gera novo mapa
		if (dir == -1 && LEVEL > 0) {
			LEVEL--;
		}
		if (dir == 1 && LEVEL < num_levels - 1) {
			LEVEL++;
		}
		if (LEVEL == num_levels) state->gameOver = 2; // ganhou o jogo 
		else if (w[LEVEL].created == 0) {
			gen_map(w[LEVEL].map,r,c);
			gen_lava(w[LEVEL].map,r,c);
			gen_grass(w[LEVEL].map,r,c);
			gen_water(w[LEVEL].map,r,c);
			w[LEVEL].mobQuantity = gen_mobs(&w[LEVEL].mobs, w[LEVEL].map, r,c, LEVEL);
			w[LEVEL].created = 1;
		}
		clear();
		while (w[LEVEL].map[state->player.position.y][state->player.position.x].object != 0 && (!(w[LEVEL].map[state->player.position.y][state->player.position.x].object == 2 && (LEVEL == 0 || LEVEL == 9)))) {
			state->player.position.x = (random() % c);
			state->player.position.y = (random() % r);

		}
	} 
}

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


int game(Terminal *terminal) {
	clear();
	GameState *gameState = init_game_state();

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
		worlds[i].bullets = (Bullet *)malloc(sizeof(Bullet));
		worlds[i].bulletQuantity = 0;
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
	gen_grass(worlds[LEVEL].map,nrows,ncols); // no nível 0 é possível existir relva
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
			//draw_light(gameState, nrows, ncols, worlds[LEVEL].map, terminal);

			for(int i = 0; i < worlds[LEVEL].bulletQuantity; i++){
				draw_bullet(&worlds[LEVEL].bullets[i], terminal);
			}

			// Botões (temporário)
			int buttonToolbarX = (terminal->xMax / 2) - (73 / 2);
			Vector2D buttonExplorePos = {buttonToolbarX,terminal->yMax-1};
			button(buttonGradient, "Explore", buttonExplorePos);
			Vector2D buttonRestPos = {buttonToolbarX+13+4,terminal->yMax-1};
			button(buttonGradient, "Rest", buttonRestPos);
			Vector2D buttonSearchPos = {buttonToolbarX+13+4+14,terminal->yMax-1};
			button(buttonGradient, "Search", buttonSearchPos);
			Vector2D buttonMenuPos = {buttonToolbarX+12+4+13+4+14,terminal->yMax-1};
			button(buttonGradient, "Menu", buttonMenuPos);
			Vector2D buttonInvPos = {buttonToolbarX+14+12+4+13+4+14,terminal->yMax-1};
			button(buttonGradient, "Inventory", buttonInvPos);

			if (gameState->gameOver == 1){
				move(0,150);
				printw("** PERDEU O JOGO Prima c para continuar **");
				refresh();
				int c;
				do {
					c = getchar();
				} while (c != 'c');
				endwin();
				return(0);
			}
			if (gameState->gameOver == 2){
				move(0,150);
				printw("** GANHOU O JOGO Prima c para continuar **");
				refresh();
				int c;
				do {
					c = getchar();
				} while (c != 'c');
				endwin();
				return(0);
			}

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
					mvaddch(0, 0, ' ');
				}
			}

			Vector2D enemyInfoPos = { 0, 5 };
			enemy_info_ui(gameState, &worlds[LEVEL], enemyInfoPos, terminal);

			mvprintw(0, 180, "Level: %d", LEVEL);
		}

		update(gameState, worlds, nrows, ncols, currentTime, terminal);
	}

	return 0;
}