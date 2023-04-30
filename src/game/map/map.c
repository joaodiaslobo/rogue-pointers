#include "map.h"
#include <string.h>
#include <ncurses.h>
#include "game_types.h"
#include "image.h"
#include "draw.h"
#include "game.h"

// Necessário para as funções que geram o mapa

void gen_grass(MAP** a, int r, int c) {
	// decidir se aparece relva em um nível
	int x = 0, y = 0, grass = 0, ind, n = 3;
	int prob_grass = 90 - 10*LEVEL;
    while (n > 0) {
		grass = 0;
		int random_num = rand() % 100;
   		if (random_num <= prob_grass) {
			grass = 1;
		}
		// gerar a relva
		if (grass == 1) {
			while(a[y][x].object != 0) {
				x = (random() % c);
        		y = (random() % r);
			}
			a[y][x].object = 5;
			a[y][x+1].object = 5;
			a[y][x-1].object = 5;
			a[y+1][x].object = 5;
			a[y-1][x].object = 5;
			for(int i = 1; i < r-1; i++) {    
	    		for(int j = 1; j < c-1; j++) {
						if (a[i][j].object == 5) {
						int conditions[] = {a[i][j+1].object, a[i][j-1].object, a[i+1][j].object, a[i-1][j].object};
						// dependendo da profundidade do nível é gerada mais relva
						if (LEVEL >= 8 && LEVEL < 9){
					 		ind = (random() % 3);
					  	    if (conditions[ind] == 0) conditions[ind] = 5;	
						} else if (LEVEL >= 6 && LEVEL < 8){
							ind = (random() % 3);
					  	    if (conditions[ind] == 0) conditions[ind] = 5;
							ind = (random() % 3);
					 		if (conditions[ind] == 0) conditions[ind] = 5;
						} else if (LEVEL >= 3 && LEVEL < 5){
							ind = (random() % 3);
					   		if (conditions[ind] == 0) conditions[ind] = 5;
							ind = (random() % 3);
					    	if (conditions[ind] == 0) conditions[ind] = 5;
							ind = (random() % 3);
					    	if (conditions[ind] == 0) conditions[ind] = 5;
						} else if (LEVEL >= 0 && LEVEL <= 2) {
							ind = (random() % 3);
					    	if (conditions[ind] == 0) conditions[ind] = 5;
							ind = (random() % 3);
					    	if (conditions[ind] == 0) conditions[ind] = 5;
							ind = (random() % 3);
					    	if (conditions[ind] == 0) conditions[ind] = 5;
							ind = (random() % 3);
					    	if (conditions[ind] == 0) conditions[ind] = 5;
						}
						if (conditions[0] == 5) a[i][j+1].object = 5;
						if (conditions[1] == 5) a[i][j-1].object = 5;
						if (conditions[2] == 5) a[i+1][j].object = 5;
						if (conditions[3] == 5) a[i-1][j].object = 5;
					}
				}
			}
		}
		n--;
	}
}


void gen_lava(MAP** a, int r, int c) {
	// decidir se aparece lava em um nível
	int x = 0, y = 0, lava = 0, ind;
	int prob_lava = 10 * LEVEL;
    int random_num = rand() % 100;
    if (random_num <= prob_lava && LEVEL != 0) {
		lava = 1;
	}
	// gerar a lava
	if (lava == 1) {
		while(a[y][x].object != 0) {
			x = (random() % c);
        	y = (random() % r);
		}
		a[y][x].object = 4;
		a[y][x+1].object = 4;
		a[y][x-1].object = 4;
		a[y+1][x].object = 4;
		a[y-1][x].object = 4;

		for(int i = 1; i < r-1; i++) {    
	    	for(int j = 1; j < c-1; j++) {
				if (a[i][j].object == 4) {
					int conditions[] = {a[i][j+1].object, a[i][j-1].object, a[i+1][j].object, a[i-1][j].object};
					// dependendo da profundidade do nível é gerada mais lava
					if (LEVEL >= 1 && LEVEL < 3){
					    ind = (random() % 3);
					    if (conditions[ind] == 0) conditions[ind] = 4;	
					} else if (LEVEL >= 3 && LEVEL < 5){
						ind = (random() % 3);
					    if (conditions[ind] == 0) conditions[ind] = 4;
						ind = (random() % 3);
					    if (conditions[ind] == 0) conditions[ind] = 4;
					} else if (LEVEL >= 5 && LEVEL < 7){
						ind = (random() % 3);
					    if (conditions[ind] == 0) conditions[ind] = 4;
						ind = (random() % 3);
					    if (conditions[ind] == 0) conditions[ind] = 4;
						ind = (random() % 3);
					    if (conditions[ind] == 0) conditions[ind] = 4;
					} else if (LEVEL >= 7 && LEVEL <= 9) {
						ind = (random() % 3);
					    if (conditions[ind] == 0) conditions[ind] = 4;
						ind = (random() % 3);
					    if (conditions[ind] == 0) conditions[ind] = 4;
						ind = (random() % 3);
					    if (conditions[ind] == 0) conditions[ind] = 4;
						ind = (random() % 3);
					    if (conditions[ind] == 0) conditions[ind] = 4;
					}
					if (conditions[0] == 4) a[i][j+1].object = 4;
					if (conditions[1] == 4) a[i][j-1].object = 4;
					if (conditions[2] == 4) a[i+1][j].object = 4;
					if (conditions[3] == 4) a[i-1][j].object = 4;
				}
			}
		}
	}
}

void new_room_map (MAP** a, int r, int c){
    int random_rooms = (random() % 11) + 15; // podemos ter entre 15 a 25 salas
    int k; 
    while (k < random_rooms) {
        int width_room = (random() % 13) + 14; // largura da sala (14 a 26)
        int height_room = (random() % 13) + 10; // altura da sala (10 a 22)
        int roomX = (random() % c) + 1; // posição da sala no mapa
        int roomY = (random() % r) + 1;
        
		// verifica se a sala fica situada dentro do mapa, senão gera nova sala
		if((roomX + width_room) < c-1 && (roomY + height_room < r-1)){  
            // coloca paredes na sala
            for(int j = roomX; j < roomX + width_room+1; j++){ // constroi a border horizontal da sala
		      a[roomY][j].object = 1; 
		      a[roomY+height_room][j].object = 1;
	        } 
            for(int i = roomY; i < roomY + height_room; i++) { // constroi a border vertical da sala
              a[i][roomX].object = 1; 
	          a[i][roomX+width_room].object = 1;
	        }
            // coloca interior da salas como espaço em que é possível andar
			for (int i = roomY + 1; i < roomY + height_room; i++) {
               for (int j = roomX + 1; j < roomX + width_room; j++) {
                   a[i][j].object = 0;
               }
            }
		    // abre portas entre salas adjacentes
			int init = roomY + height_room, end = 0;
            for (int j = roomY; j < roomY + height_room && j != 1; j++){ // esquerda
		      while (a[j][roomX].object == 1 && a[j][roomX-1].object != 3 && a[j][roomX+1].object != 3 && a[j][roomX-1].object != 1){ //tem de abrir porta
			 	  if (init > j) init = j;
				  if (end < j) end = j;
				  j++;
			   }
			   if (end - init == 1 && a[init][roomX+1].object != 1 && a[end][roomX+1].object != 1){ // para abrir uma porta necessita de ter pelo menos duas posições disponíveis e no ultimo nao podemos ter uma parede
			      a[init][roomX].object = 0;
				  a[end][roomX].object = 0;
			   }
			   else if (end - init >= 2){
			      int door = (random() % (end-init))+1;
				  if (init+door+1 < end && a[init+door][roomX+1].object != 1 && a[init+door+1][roomX+1].object != 1 && a[init+door][roomX-1].object != 1 && a[init+door+1][roomX-1].object != 1){
				      a[init+door][roomX].object = 0;
					  a[init+door+1][roomX].object = 0;
				  }
				  else if (a[init+door-1][roomX+1].object != 1 && a[init+door-2][roomX+1].object != 1 && a[init+door-1][roomX-1].object != 1 && a[init+door-2][roomX-1].object != 1){ 
				     a[init+door-1][roomX].object = 0;
					 a[init+door-2][roomX].object = 0;
			      }
			   }
               end = 0;
			   init = j+1;
			}
		    init = roomY + height_room, end = 0;
            for (int j = roomY; j < roomY + height_room && j != r-1; j++){ // direita
		       while (a[j][roomX+width_room].object == 1 && a[j][roomX+width_room+1].object != 3 && a[j][roomX+width_room-1].object != 3 && a[j][roomX+width_room+1].object != 1){
			 	  if (init > j) init = j;
				  if (end < j) end = j;
				  j++;
			   }
			   if(end - init == 1 && a[init][roomX+width_room-1].object != 1 && a[end][roomX+width_room-1].object != 1){
			      a[init][roomX+width_room].object = 0;
				  a[end][roomX+width_room].object = 0;
			   }
			   else if(end - init >= 2){
			      int door = (random() % (end-init))+1;
				  if(init+door+1 < end && a[init+door][roomX+width_room+1].object != 1 && a[init+door+1][roomX+width_room+1].object != 1 && a[init+door][roomX+width_room-1].object != 1 && a[init+door+1][roomX+width_room-1].object != 1){ 
				     a[init+door][roomX+width_room].object = 0;
					 a[init+door+1][roomX+width_room].object = 0;
				  }
				  else if(a[init+door-1][roomX+width_room+1].object != 1 && a[init+door-2][roomX+width_room+1].object != 1 && a[init+door-1][roomX+width_room-1].object != 1 && a[init+door-2][roomX+width_room-1].object != 1){
				     a[init+door-1][roomX+width_room].object = 0;
					 a[init+door-2][roomX+width_room].object = 0;
				  }
			   }
			   end = 0;
			   init = j+1;
			}
		    init = roomX + width_room, end = 0;
            for(int j = roomX; j < roomX + width_room && j > 0; j++){ // cima
		       while (a[roomY][j].object == 1 && a[roomY-1][j].object != 3 && a[roomY+1][j].object != 3 && a[roomY-1][j].object != 1){
			 	  if (init > j) init = j;
				  if (end < j) end = j;
				  j++;
			   }
			   if(end - init == 1 && a[roomY+1][init].object != 1 && a[roomY+1][end].object != 1){
			      a[roomY][init].object = 0;
				  a[roomY][end].object = 0;
			   }
			   else if (end - init >= 2){
			      int door = (random() % (end-init))+1;
				  if(init+door+1 < end && a[roomY-1][init+door].object != 1 && a[roomY-1][init+door+1].object != 1 && a[roomY+1][init+door].object != 1 && a[roomY+1][init+door+1].object != 1){ 
				     a[roomY][init+door].object = 0;
					 a[roomY][init+door+1].object = 0;
				  }
				  else if(a[roomY-1][init+door-1].object != 1 && a[roomY-1][init+door-2].object != 1 && a[roomY+1][init+door-1].object != 1 && a[roomY+1][init+door-2].object != 1){ 
				     a[roomY][init+door-1].object = 0;
					 a[roomY][init+door-2].object = 0;
				  }
			  }
			  end = 0;
			  init = j+1;
			}
		    init = roomX + width_room, end = 0;
            for(int j = roomX; j < roomX + width_room && j != c; j++){ // baixo
		       while (a[roomY+height_room][j].object == 1 && a[roomY+height_room+1][j].object != 3 && a[roomY+height_room-1][j].object != 3 && a[roomY+height_room+1][j].object != 1){
			 	  if (init > j) init = j;
				  if (end < j) end = j;
				  j++;
			   }
			   if(end - init == 1 && a[roomY+height_room+1][init].object != 1 && a[roomY+height_room+1][end].object != 1){
			      a[roomY+height_room][init].object = 0;
				  a[roomY+height_room][end].object = 0;
			   }
			   else if (end - init >= 2){
			      int door = (random() % (end-init))+1;
				  if(init+door+1 < end && a[roomY+height_room+1][init+door].object != 1 && a[roomY+height_room+1][init+door+1].object != 1 && a[roomY+height_room-1][init+door].object != 1 && a[roomY+height_room-1][init+door+1].object != 1){ 
				     a[roomY+height_room][init+door].object = 0;
					 a[roomY+height_room][init+door+1].object = 0;
				  }
				  else if(a[roomY+height_room-1][init+door-1].object != 1 && a[roomY+height_room-1][init+door-2].object != 1 && a[roomY+height_room-1][init+door+1].object != 1 && a[roomY+height_room+1][init+door-2].object != 1){ 
				     a[roomY+height_room][init+door-1].object = 0;
					 a[roomY+height_room][init+door-2].object = 0;
				  }
			   }
			   end = 0;
			   init = j+1;
			}
        	k++;
		}
    }
}

void new_level_map (MAP** a, int r, int c) {
	int random_num, count = 0, rc, rr;

	random_num = (random() % 3) + 1;
	while(count < random_num) {
		rc = (random() % c);
		rr = (random() % r);
		if (a[rr][rc].object == 0 && (a[rr-1][rc].object==0 || a[rr+1][rc].object==0 || a[rr][rc-1].object==0 || a[rr][rc+1].object==0)){
			a[rr][rc].object = 2;
			count++;
		}
	}
}

// Gera as borders do mapa
/*
void gen_border_map(MAP (*a)[NUM_COLUMNS], int r, int c) {  
   for(int j = 0; j < c; j++){ // Constroi a border horizontal
		a[0][j].object = 1; 
		a[r-1][j].object = 1;
	} 
	
    for(int i = 0; i < r; i++) { // Constroi a border vertical
        a[i][0].object = 1; 
	    a[i][c-1].object = 1;
	} 
}
*/

// Gera a primeira versão do mapa de uma forma aleatória
/*
void gen_first_map(MAP (*a)[NUM_COLUMNS], int r, int c) {  
   int i = 0;

   for(i = 1; i < r-1; i++) {    
	    for(int j = 1; j < c-1; j++) {
           int valid_objects[] = {1, 1, 1, 0, 0}; //60% de paredes e 40% de espaços por onde andar
           int random_num = (random() % 5);
           a[i][j].object = valid_objects[random_num];
		}
   } 
}
*/

// Aliza o mapa
/*void smooth_map(MAP (*a)[NUM_COLUMNS], int r, int c, int x1, int x2) {
   int count = 0, i;
   
   for(i = 2; i < r-2; i = i+3) {    
	    for(int j = 2; j < c-2; j = j+3) {
		   if (a[i][j].object == 1){ // conta
		      if (a[i][j-1].object == 0) count++;
			  if (a[i][j+1].object == 0) count++;
			  if (a[i-1][j].object == 0) count++;
			  if (a[i+1][j].object == 0) count++;
			  if (a[i-1][j-1].object == 0) count++;
			  if (a[i-1][j+1].object == 0) count++; 
			  if (a[i+1][j-1].object == 0) count++;
			  if (a[i+1][j+1].object == 0) count++;
		   }
		   //printw("contador:%d ",count);
		   //getch();
		   if (count >= x1){
			  a[i][j].object = 1;
			  a[i][j-1].object = 1;
			  a[i][j+1].object = 1;
			  a[i-1][j].object = 1;
			  a[i+1][j].object = 1;
			  a[i+1][j+1].object = 1;
			  a[i+1][j-1].object = 1;
			  a[i-1][j+1].object = 1;
			  a[i-1][j-1].object = 1;
		   } else if (count <= x2){
			  a[i][j].object = 0;
			  a[i][j-1].object = 0;
			  a[i][j+1].object = 0;
			  a[i-1][j].object = 0;
			  a[i+1][j].object = 0;
			  a[i+1][j+1].object = 0;
			  a[i+1][j-1].object = 0;
			  a[i-1][j+1].object = 0;
			  a[i-1][j-1].object = 0;
		   } 
		   count = 0;
		}
    }
}
*/

// Gera o mapa
void gen_map(MAP** a, int r, int c) {
   
   /*
      gen_border_map(a,r,c);
      gen_first_map(a,r,c);
      smooth_map(a,r,c,5,2);
	  new_level_map(a,r,c);
   */
   
    new_room_map(a,r,c);
    new_level_map(a,r,c);
}

// Imprime o mapa
void print_map(MAP** a, int r, int c) {
   Image wall = load_image_from_file("assets/sprites/wall.sprite");
   Image gate = load_image_from_file("assets/sprites/gate.sprite");
   Image walk = load_image_from_file("assets/sprites/walk.sprite");
   Image lava = load_image_from_file("assets/sprites/lava.sprite");
   Image grass = load_image_from_file("assets/sprites/grass.sprite");

   for (int i = 0; i < r; i++){
      for (int j = 0; j < c; j++){
		if (a[i][j].object == 0){
		    //Vector2D pos = {j,i};
			//draw_to_screen(walk, pos);
			int k = 100;
			init_pair(k, COLOR_BLACK, walk.pixels[0].color);         
			attron(COLOR_PAIR(k));
			mvprintw(i, j*2, "  ");
            attroff(COLOR_PAIR(k));
		}
		else if(a[i][j].object == 1){ // imprimir a parede 
			//Vector2D pos = {j,i};
			//draw_to_screen(wall, pos);
			    int k = 101;
			    init_pair(k, COLOR_BLACK, wall.pixels[0].color);         
				attron(COLOR_PAIR(k));
				mvprintw(i, j*2, "  ");
                attroff(COLOR_PAIR(k));
			//move(i,j*2);
			//printw("#");
		}
		else if(a[i][j].object == 2){ // imprimir porta para outro nível
			//Vector2D pos = {j,i};
			//draw_to_screen(gate, pos);
			int k = 102;
			init_pair(k, COLOR_BLACK, gate.pixels[0].color);         
			attron(COLOR_PAIR(k));
			mvprintw(i, j*2, "  ");
            attroff(COLOR_PAIR(k));
		}
		else if(a[i][j].object == 4){ 
			int r = random() % 5;
			int k = 103;
			init_pair(k, COLOR_BLACK, lava.pixels[r].color);         
			attron(COLOR_PAIR(k));
			mvprintw(i, j*2, "  ");
            attroff(COLOR_PAIR(k));
		}
		else if(a[i][j].object == 5){ 
			int k = 104;
			init_pair(k, COLOR_BLACK, grass.pixels[0].color);         
			attron(COLOR_PAIR(k));
			mvprintw(i, j*2, "**" );
            attroff(COLOR_PAIR(k));
		}
      }
    }
}