#include "game.h"
#include <ncurses.h>
#include <stdlib.h>
#include "map.h"
#include "time.h"
#include "game_types.h"
#include "engine_types.h"
#include "player.h"
#include "image.h"
#include "draw.h"

// TODO: Este ficheiro tem coisas a mais, algumas funcionalidades devem ser separadas para uma pasta à parte

int NUM_COLUMNS;

<<<<<<< HEAD
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
=======
/**
 *
 * Um pequeno exemplo que mostra o que se pode fazer
 */
void do_movement_action(STATE *st, int dx, int dy, MAP (*m)[NUM_COLUMNS], int r, int c) {
    int x = st->playerX + dx;
	int y = st->playerY + dy;
    
	//attron(COLOR_PAIR(COLOR_BLUE));
	move(r-1,20);
	printw("col:%d lin:%d tc:%d tl:%d Obs:%d col:%d, lin:%d", st->playerX, st->playerY, c, r, m[y][x].object, x,y);
	//getch();
	//attroff(COLOR_PAIR(COLOR_BLUE));
	 	
	if (m[y][x].object == 1 || m[y][x].object == 3 || x < 0 || y < 0 || y >= r || x >= c){ 
		// valida a posição do jogador no mapa, se este move para uma parede não pode avançar
        // valida que o jogador não sai do mapa
	} else if (m[y][x].object == 2) { //encontrou um porta, muda de nível e gera novo mapa
        gen_map(m,r,c);
	    print_map(m,r,c);   
	} 
      else {
          st->playerX += dx;
	      st->playerY += dy;
   	  }
>>>>>>> c7ff2cb (Adaptação do jogo a vários níveis)
}


void execute_input(GameState *state, Map (*m)[MAX_MAP_COLUMNS], int r, int c){
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

void check_for_portal(GameState *state, Map (*m)[MAX_MAP_COLUMNS], int r, int c){
	if (m[state->player.position.y][state->player.position.x].object == 2) { //encontrou um porta, muda de nível e gera novo mapa
        gen_map(m,r,c,2);
	    print_map(m,r,c);   
	} 
}

void update(GameState *state, Map (*m)[MAX_MAP_COLUMNS], int r, int c) {
	execute_input(state, m, r, c);
	check_for_portal(state, m, r, c);
}


int game(Terminal *terminal) {
	GameState *gameState = init_game_state();

	int ncols, nrows;
    ncols = terminal->xMax;
	ncols = ncols/2;
    nrows = terminal->yMax;

	NUM_COLUMNS = ncols;

<<<<<<< HEAD
	short buttonGradient[4] = {16,17,18,19};
=======
	int num_levels = 10;
    World* worlds = (World*)malloc(num_levels * sizeof(World));
	if (worlds == NULL) {
		exit(EXIT_FAILURE);
	}
    for (int i = 0; i < num_levels; i++) {
		worlds[i].level = i+1;
		worlds[i].map = (MAP**)malloc(nrows * sizeof(MAP*));
	    if (worlds[i].map == NULL) {
		   exit(EXIT_FAILURE);
	   }
	   for (int j = 0; j < nrows; j++) {
		   worlds[i].map[j] = (MAP*)malloc(ncols * sizeof(MAP));
		   if (worlds[i].map[j] == NULL) {
			   exit(EXIT_FAILURE);
		   }
		   for (int k = 0; k < ncols; k++) {
			   worlds[i].map[j][k].object = 3; // Inicializando o valor do campo object como 3 (vazio)
		   }
	   }
>>>>>>> 9002671 (Adaptação do jogo a vários níveis)
	
<<<<<<< HEAD
	// Criação e inicialização do mapa 
	
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
=======
	}
>>>>>>> c7ff2cb (Adaptação do jogo a vários níveis)

	srandom(time(NULL));

	intrflush(stdscr, false);
    
    // Gera e imprime o primeiro mapa/nível do mundo
    gen_map(worlds[0].map,nrows,ncols);
	print_map(worlds[0].map,nrows,ncols);
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
		print_map(worlds[0].map,nrows,ncols);
		Image gate = load_image_from_file("assets/sprites/gate.sprite"); //Não apagar estas 3 linhas, usadas p/ testes
	    Vector2D pos = {st.playerX,st.playerY};
<<<<<<< HEAD
	    draw_to_screen(gate, pos); */
		draw_to_screen(characterSprite, gameState->player.position);
		/*
		Se utilizarmos apenas 1 quadrado como player, as funcionalidades de não atravessar paredes e descer de níveis, 
=======
	    draw_to_screen(gate, pos); 
		//Vector2D pos = {st.playerX, st.playerY}; 
		//draw_to_screen(characterSprite, pos);
		/*Se utilizarmos apenas 1 quadrado como player, as funcionalidades de não atravessar paredes e descer de níveis, 
>>>>>>> c7ff2cb (Adaptação do jogo a vários níveis)
		já funcionam. Não consegui colocar o Dan a funcionar no contexto, pois ele não possui um centro de massa, ou seja,
		um ponto sincronizado com o mapa. Além disso, com a forma atual do Dan, na horizonal ele só percorre caminhos de 6
		e na vertical caminhos de 4, ou seja, não sei se faz propriamente sentido e, por isso, apenas tratei desta funciona-
		lidade para um ponto.
		*/
<<<<<<< HEAD
		//move(st.playerX, st.playerY);

		// Botões (temporário)
		int buttonToolbarX = (terminal->xMax / 2) - (73 / 2) ;
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

		update(gameState,mp,nrows,ncols);
	}

=======
	
		//attron(COLOR_PAIR(COLOR_BLUE));
		mvaddch(st.playerX - 1, st.playerY - 1, '.' | A_BOLD);
		mvaddch(st.playerX - 1, st.playerY + 0, '.' | A_BOLD);
		mvaddch(st.playerX - 1, st.playerY + 1, '.' | A_BOLD);
		mvaddch(st.playerX + 0, st.playerY - 1, '.' | A_BOLD);
		mvaddch(st.playerX + 0, st.playerY + 1, '.' | A_BOLD);
		mvaddch(st.playerX + 1, st.playerY - 1, '.' | A_BOLD);
		mvaddch(st.playerX + 1, st.playerY + 0, '.' | A_BOLD);
		mvaddch(st.playerX + 1, st.playerY + 1, '.' | A_BOLD);
        //attroff(COLOR_PAIR(COLOR_YELLOW));
		//move(st.playerX, st.playerY);
		update(&st,worlds[0].map,nrows,ncols);
	} 
>>>>>>> c7ff2cb (Adaptação do jogo a vários níveis)
	return 0;
}