#include <ncurses.h>
#include <stdlib.h>
#include "map/map.c"
#include "time.h"
#include "game_types.h"
#include "engine_types.h"
#include "player/player.c"

// TODO: Este ficheiro tem coisas a mais, algumas funcionalidades devem ser separadas para uma pasta à parte

GameState *init_game_state(){
	GameState *state = malloc(sizeof(GameState));
	if (state == NULL) {
        fprintf(stderr, "ERRO: Nao foi possivel alocar memoria\n");
        exit(EXIT_FAILURE);
    }
	Vector2D pos = {20,20};
	char name[15] = "NOME";
	state->player = *init_player(name, pos);
	return state;
}


void execute_input(GameState *state, Map (*m)[NUM_COLUMNS], int r, int c){
	int key = getch();
    
	switch (key)
	{
		case KEY_UP:
			apply_movement(state, NORTH, m, r, c);
			break;
		case KEY_DOWN:
			apply_movement(state, SOUTH, m, r, c);
			break;
		case KEY_RIGHT:
			apply_movement(state, EAST, m, r, c);
			break;
		case KEY_LEFT:
			apply_movement(state, WEST, m, r, c);
			break;
		case KEY_A1:
			apply_movement(state, NORTH, m, r, c);
			apply_movement(state, WEST, m, r, c);
			break;
		case KEY_A3:
			apply_movement(state, NORTH, m, r, c);
			apply_movement(state, EAST, m, r, c);
			break;
		case KEY_C1:
			apply_movement(state, SOUTH, m, r, c);
			apply_movement(state, WEST, m, r, c);
			break;
		case KEY_C3:
			apply_movement(state, SOUTH, m, r, c);
			apply_movement(state, EAST, m, r, c);
			break;
		case 'q':
			endwin();
			exit(0);
			break;
		default:
			break;
	}
}

void check_for_portal(GameState *state, Map (*m)[NUM_COLUMNS], int r, int c){
	if (m[state->player.position.y][state->player.position.x].object == 2) { //encontrou um porta, muda de nível e gera novo mapa
        gen_map(m,r,c,2);
	    print_map(m,r,c);   
	} 
}

void update(GameState *state, Map (*m)[NUM_COLUMNS], int r, int c) {
	execute_input(state, m, r, c);
	check_for_portal(state, m, r, c);
}


int game(Terminal *terminal) {
	GameState *gameState = init_game_state();

	int ncols, nrows;
    ncols = terminal->xMax;
	ncols = ncols/2;
    nrows = terminal->yMax;
	
	// Criação e inicialização do mapa 
	NUM_COLUMNS = ncols;
	
	Map (*mp)[ncols] = malloc(sizeof(Map[nrows][ncols]));
    if (mp == NULL) {
        fprintf(stderr, "ERRO: Nao foi possivel alocar memoria\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nrows; i++){
      for (int j = 0; j < ncols; j++){
         mp[i][j].object = 3;
      }
    }

	srandom(time(NULL));

	intrflush(stdscr, false);

	init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
	init_pair(COLOR_RED, COLOR_MAGENTA, COLOR_BLACK);
    
    // Gera e imprime o mapa
    gen_map(mp,nrows,ncols,2);
	print_map(mp,nrows,ncols);
	getch();
	
	endwin(); 

	
	/**
	 * Este código está muito mal escrito!
	 * Deveria existir uma função chamada draw_player!
	 *
	 * Se estamos a desenhar uma luz à volta do jogador
	 * deveria existir uma função chamada draw_light!
	 *
	 */

	Image characterSprite = load_image_from_file("assets/sprites/characters/player1.sprite");

	
	while(1) {
		move(nrows - 1, 0);
		attron(COLOR_PAIR(COLOR_BLUE));
		printw("(%d, %d) %d %d", gameState->player.position.x, gameState->player.position.y, ncols, nrows);
		attroff(COLOR_PAIR(COLOR_BLUE));
		//attron(COLOR_PAIR(COLOR_BLACK));
		//attroff(COLOR_PAIR(COLOR_RED));
		print_map(mp,nrows,ncols);
		/*Image gate = load_image_from_file("assets/sprites/gate.sprite"); Não apagar estas 3 linhas, usadas p/ testes
	    Vector2D pos = {st.playerX,st.playerY};
	    draw_to_screen(gate, pos); */
		draw_to_screen(characterSprite, gameState->player.position);
		/*
		Se utilizarmos apenas 1 quadrado como player, as funcionalidades de não atravessar paredes e descer de níveis, 
		já funcionam. Não consegui colocar o Dan a funcionar no contexto, pois ele não possui um centro de massa, ou seja,
		um ponto sincronizado com o mapa. Além disso, com a forma atual do Dan, na horizonal ele só percorre caminhos de 6
		e na vertical caminhos de 4, ou seja, não sei se faz propriamente sentido e, por isso, apenas tratei desta funciona-
		lidade para um ponto.
		*/
		//move(st.playerX, st.playerY);

		update(gameState,mp,nrows,ncols);
	}

	return 0;
}