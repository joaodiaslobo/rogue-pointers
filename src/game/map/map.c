#include "map.h"
#include <string.h>
#include <ncurses.h>
#include <stdlib.h>
#include "game_types.h"
#include "image.h"
#include "draw.h"
#include "game.h"
#include "engine_types.h"
#include "stdlib.h"

// Necessário para as funções que geram o mapa

void chest_room(MAP** a, int r, int c) {
	for(int i = 1; i < r-1; i++) {  
		for(int j = 1; j < c-1; j++) {
			Vector2D v1 = {0, 0}, d1 = {0, 0}, d2 = {0, 0};
			int l1 = 1, l2 = 1, l3 = 1, l4 = 1, num_doors = 0, aux1 = 0, aux2 = 0;
			if(a[i][j].object == 1 && a[i][j+1].object == 1 && a[i+1][j].object == 1){ //encontra o canto superior esquerdo da sala
				v1.x = j;
				v1.y = i;
            	
				aux1 = i+1;
				while(a[aux1][j+1].object != 1 && aux1 < r-1){ //percorre parede esquerda da sala (lado 1)
					l1 = l1+1;
					if(a[aux1][j].object == 0){ 
						num_doors++; //encontrou uma porta
						if(num_doors == 2){ //guarda posição da porta
							d1.x = j;
							d1.y = aux1-1;
							d2.x = j;
							d2.y = aux1;
						}
					} 
					aux1++;	
				}
				aux2 = j+1;
				while(a[i+1][aux2].object != 1 && aux2 < c-1){ //percorre parede superior da sala (lado 2)
					l2 = l2+1;
					if(a[i][aux2].object == 0){ 
						num_doors++; //encontrou uma porta
						if(num_doors == 2){ //guarda posição da porta
							d1.x = aux2-1;
							d1.y = i;
							d2.x = aux2;
							d2.y = i;
						}
					} 
					aux2++;	
				}
				j++;
				while(a[aux1-1][j].object != 1 && j < c-1){ //percorre parede inferior da sala (lado 3)
					l3 = l3+1;
					if(a[aux1][j].object == 0){ 
						num_doors++; //encontrou uma porta
						if(num_doors == 2){ //guarda posição da porta
							d1.x = j-1;
							d1.y = aux1;
							d2.x = j;
							d2.y = aux1;
						}
					} 
					j++;	
				}
				aux1 = i+1;
				while(a[aux1][j-1].object != 1 && aux1 < r-1){ //percorre parede direita da sala (lado 4)
					l4 = l4+1;
					if(a[aux1][j].object == 0){ 
						num_doors++; //encontrou uma porta
						if(num_doors == 2){ //guarda posição da porta
							d1.x = j;
							d1.y = aux1-1;
							d2.x = j;
							d2.y = aux1;
						}
					} 
					aux1++;
				}
				if(num_doors == 2 && l1 == l4 && l2 == l3){
					for(int k = v1.y + 1; k < v1.y + l1; k++) { //muda a cor do chão da sala fechada  
						for(int l = v1.x + 1 ; l < v1.x + l2; l++) {
							a[k][l].object = 12;
						}
					}
					a[v1.y+1][v1.x+1].object = 9; // posiciona o baú perto do canto superior esquerdo
					a[d1.y][d1.x].object = 10; // fecha a porta - duas posições
					a[d2.y][d2.x].object = 10; // fecha a porta - duas posições
                    
					// posiciona a chave
					int x = (random() % c);
        			int y = (random() % r);
					while(a[y][x].object != 0 || (x > v1.x && x < v1.x + l2) || (y > v1.y && y < v1.y + l1)){ // garante que a chave não fica na mesma sala do baú
						x = (random() % c); 	
        				y = (random() % r);
					}
					a[y][x].object = 11;
					return;
				}
			}
		}
	}
}

int elem(Vector2D e, Vector2D *v, int N) {
    int i, ans = 0;
    for(i = 0; i < N && !ans; i++)
        if(e.x == v[i].x && e.y == v[i].y)
            ans = 1;
    return ans;
}

Vector2D* remove_elem(Vector2D* old, int old_size) {
    if (old == 0 || old_size <= 1) { // verifica se o array_antigo tem pelo menos um elemento
	    return old;
    }
    Vector2D* new= (Vector2D*) malloc((old_size-1)*sizeof(Vector2D));
    for (int i = 1; i < old_size; i++) { // copia os elementos do vetor_antigo a partir do segundo elemento para o vetor_novo
        new[i - 1] = old[i];
    }
	if (new != NULL) free(new);    
    return &old[1]; // Retorna o ponteiro para o vetor_antigo sem o primeiro elemento
}

int valid_map(MAP** a, int r, int c) {
	Vector2D fst, tmp;
	int i, count = 0, k = 0;

	fst.x = -1;
	fst.y = -1;
	for(i = 1; i < r-1; i++) { // encontra o primeiro ponto e guarda num array todas as posições onde o jogador pode andar
		for(int j = 1; j < c-1; j++) {
			if (a[i][j].object == 0){
				count++;
			    if (fst.x == -1) { // não volta a fazer esta condição depois de encontrar um primeiro ponto
					fst.x = j; 
					fst.y = i;
				}
			}
		}
	}
	i = 0;
	Vector2D* visit = malloc(count * sizeof(Vector2D));
	if (visit == NULL) {
		exit(EXIT_FAILURE);
	}
	Vector2D* walk = malloc(count * sizeof(Vector2D));
	if (walk == NULL) {
		exit(EXIT_FAILURE);
	}
	visit[i].x = fst.x;
	visit[i].y = fst.y;
	while (i != -1) { 
		tmp.x = visit[0].x;
		tmp.y = visit[0].y+1;
		if (a[tmp.y][tmp.x].object == 0 && elem(tmp, walk, count) == 0) {
			walk[k].x = tmp.x;
			walk[k].y = tmp.y;
			i++;
			visit[i].x = tmp.x;
			visit[i].y = tmp.y;
			k++;
		}
		tmp.x = visit[0].x;
		tmp.y = visit[0].y-1; 
		if (a[tmp.y][tmp.x].object == 0 && elem(tmp, walk, count) == 0) {
			walk[k].x = tmp.x;
			walk[k].y = tmp.y;
			i++;
			visit[i].x = tmp.x;
			visit[i].y = tmp.y;
			k++;
		}
		tmp.x = visit[0].x+1;
		tmp.y = visit[0].y; 
		if (a[tmp.y][tmp.x].object == 0 && elem(tmp, walk, count) == 0) {
			walk[k].x = tmp.x;
			walk[k].y = tmp.y;
			i++;
			visit[i].x = tmp.x;
			visit[i].y = tmp.y;
			k++;
		}
		tmp.x = visit[0].x-1;
		tmp.y = visit[0].y; 
		if (a[tmp.y][tmp.x].object == 0 && elem(tmp, walk, count) == 0) {
			walk[k].x = tmp.x;
			walk[k].y = tmp.y;
			i++;
			visit[i].x = tmp.x;
			visit[i].y = tmp.y;
			k++;
		}
		visit = remove_elem (visit, i+1);
		i--;
	} 
	//if (visit != NULL)  free(visit);
	//if (walk != NULL)  free(walk);
	if (k >= count) return 0; // 0 - mapa valido | 1 - Mapa inválido
	else return 1;
}


void gen_water(MAP** a, int r, int c) {
	// decidir se aparece água em um nível
	int x = 0, y = 0, water = 0, ind, n = 2, prob_water = 0;
	if (LEVEL != 0) prob_water = (1/LEVEL)*100;
    while (n > 0) {
		water = 0;
		int random_num = rand() % 100;
   		if (prob_water <= random_num) {
			water = 1;
		}
		if (water == 1) {
			while(a[y][x].object != 0) {
				x = (random() % c);
        		y = (random() % r);
			}
			a[y][x].object = 7;
			if(a[y][x+1].object == 0) a[y][x+1].object = 7;
			if(a[y][x-1].object == 0) a[y][x-1].object = 7;
			if(a[y+1][x].object == 0) a[y+1][x].object = 7;
			if(a[y-1][x].object == 0) a[y-1][x].object = 7;
			for(int i = 1; i < r-1; i++) {    
	    		for(int j = 1; j < c-1; j++) {
						if (a[i][j].object == 7) {
						int conditions[] = {a[i][j+1].object, a[i][j-1].object, a[i+1][j].object, a[i-1][j].object};
						// dependendo da profundidade do nível é gerada menos água
						if (LEVEL >= 8 && LEVEL < 9){
					 		ind = (random() % 3);
					  	    if (conditions[ind] == 0) conditions[ind] = 7;	
						} else if (LEVEL >= 6 && LEVEL < 8){
							ind = (random() % 3);
					  	    if (conditions[ind] == 0) conditions[ind] = 7;
							ind = (random() % 3);
					 		if (conditions[ind] == 0) conditions[ind] = 7;
						} else if (LEVEL >= 3 && LEVEL < 5){
							ind = (random() % 3);
					   		if (conditions[ind] == 0) conditions[ind] = 7;
							ind = (random() % 3);
					    	if (conditions[ind] == 0) conditions[ind] = 7;
							ind = (random() % 3);
					    	if (conditions[ind] == 0) conditions[ind] = 7;
						} else if (LEVEL >= 0 && LEVEL <= 2) {
							ind = (random() % 3);
					    	if (conditions[ind] == 0) conditions[ind] = 7;
							ind = (random() % 3);
					    	if (conditions[ind] == 0) conditions[ind] = 7;
							ind = (random() % 3);
					    	if (conditions[ind] == 0) conditions[ind] = 7;
							ind = (random() % 3);
					    	if (conditions[ind] == 0) conditions[ind] = 7;
						}
						if (conditions[0] == 7) a[i][j+1].object = 7;
						if (conditions[1] == 7) a[i][j-1].object = 7;
						if (conditions[2] == 7) a[i+1][j].object = 7;
						if (conditions[3] == 7) a[i-1][j].object = 7;
					}
				}
			}
		}
		n--;
	}
	// coloca margens com tipo diferente para poder associar também uma cor diferente
	for (int i = 1; i < r-1; i++) {    
	    for (int j = 1; j < c-1; j++) {
			if (a[i][j].object == 7 && (a[i+1][j].object == 0 || a[i-1][j].object == 0 || a[i][j+1].object == 0 || a[i][j-1].object == 0 || a[i+1][j].object == 5 || a[i-1][j].object == 5 || a[i][j+1].object == 5 || a[i][j-1].object == 5 || a[i+1][j].object == 6 || a[i-1][j].object == 6 || a[i][j+1].object == 6 || a[i][j-1].object == 6)){
				a[i][j].object = 8;
			} 
		}
	}
}

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
			if(a[y][x+1].object == 0) a[y][x+1].object = 5;
			if(a[y][x-1].object == 0) a[y][x-1].object = 5;
			if(a[y+1][x].object == 0) a[y+1][x].object = 5;
			if(a[y-1][x].object == 0) a[y-1][x].object = 5;
			for(int i = 1; i < r-1; i++) {    
	    		for(int j = 1; j < c-1; j++) {
						if (a[i][j].object == 5) {
						int conditions[] = {a[i][j+1].object, a[i][j-1].object, a[i+1][j].object, a[i-1][j].object};
						// dependendo da profundidade do nível é gerada menos relva
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
	// coloca flores do meio da relva
	for (int i = 1; i < r-1; i++) {    
	    for (int j = 1; j < c-1; j++) {
			if (a[i][j].object == 5){
				int k = (random() % 10);
				if (k == 0) a[i][j].object = 6;
			} 
		}
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
		if(a[y][x+1].object == 0) a[y][x+1].object = 4;
		if(a[y][x-1].object == 0) a[y][x-1].object = 4;
		if(a[y+1][x].object == 0) a[y+1][x].object = 4;
		if(a[y-1][x].object == 0) a[y-1][x].object = 4;

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
    int k = 0; 
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

/*int gen_loot_chests(Chest *chest, MAP **map, int r, int c, int level){
	
	if(level >= 1){
		
		int numChest = (random() % (level * 4)) + 1;
		
		for(int i = 0; i < numChest; i++){

			chest[i].position = get_random_floor_position(map, r, c);
			
			for(int j = 0; j < 4; j++){
				chest[i].item[j].name = "";
				chest[i].item[j].type = ;
				chest[i].item[j].description = "";
			}
		}
	}
	
	return numChest;
}*/

int gen_mobs(Mob *mobs, MAP **map, int r, int c, int level){
	// Isto pode servir para fazer um modo dificíl mais tarde, subindo o valor
	int mobSpawnRate = 2;
	// Só spawna mobs a partir do segundo nível
	if(level > 0){
		int numMobs = (random() % (level * mobSpawnRate)) + 1;
		
		for(int i = 0; i < numMobs; i++){
			Mob mob;
			mob.position = get_random_floor_position(map, r, c);
			mob.targetPosition = mob.position;
			mob.health = 10;
			mob.mobBehavior = STUPID;
			mob.timeSinceLastUpdate = 0;
			mob.pathStep = 0;
			mob.path = NULL;
			mob.chasingPlayer = 0;
			mobs[i] = mob;
		}

		return numMobs;
	}

	return 0;
}

void new_level_map (MAP** a, int r, int c){
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

Vector2D get_random_floor_position(MAP** map, int r, int c){
	int available = 0;
	Vector2D pos;
	while(!available){
		int randX = (random() % c);
		int randY = (random() % r);
		if(map[randY][randX].object == 0){
			pos.x = randX;
			pos.y = randY;
			available = 1;
		}
	}
	return pos;
}

// Gera o mapa
void gen_map(MAP** a, int r, int c) {
    new_room_map(a,r,c);
	while (valid_map(a,r,c) == 1) {
		for (int j = 0; j < r; j++) {
		   for (int k = 0; k < c; k++) {
				a[j][k].object = 3; // Inicializando o valor do campo object como 3 (vazio)
		   }
	   	}
		new_room_map(a,r,c);
	}
    new_level_map(a,r,c);
	chest_room(a,r,c);
}

void draw_mobs(Mob *mobs, int r, int c, int mobQuantity){
	Image enemy = load_image_from_file("assets/sprites/characters/enemy.sprite");

	for(int i = 0; i < mobQuantity; i++){
		draw_to_screen(enemy, mobs[i].position);
	}
}

// Imprime o mapa
void print_map(MAP** a, int r, int c) {
   Image wall = load_image_from_file("assets/sprites/wall.sprite");
   Image gate = load_image_from_file("assets/sprites/gate.sprite");
   Image walk = load_image_from_file("assets/sprites/walk.sprite");
   Image lava = load_image_from_file("assets/sprites/lava.sprite");
   Image grass = load_image_from_file("assets/sprites/grass.sprite");
   Image water = load_image_from_file("assets/sprites/water.sprite");
   Image chest = load_image_from_file("assets/sprites/chest.sprite");
   Image door = load_image_from_file("assets/sprites/door.sprite");
   Image key = load_image_from_file("assets/sprites/key.sprite");
   Vector2D pos;	
   int k = 0, r_num = 0;
   for (int i = 0; i < r; i++){
      for (int j = 0; j < c; j++){
		switch (a[i][j].object){
			case 0: // imprime lugar onde o jogador pode andar
		    	//pos.x = j;
				//pos.y = i;
				//draw_to_screen(walk, pos);
				k = 100;
				init_pair(k, COLOR_BLACK, walk.pixels[0].color);          
				attron(COLOR_PAIR(k));
				mvprintw(i, j*2, "  " );
            	attroff(COLOR_PAIR(k));
				break;
			case 1: // imprimir a parede 
				pos.x = j;
				pos.y = i;
				draw_to_screen(wall, pos);
				break;
			case 2: // imprimir porta para outro nível
				pos.x = j;
				pos.y = i;
				draw_to_screen(gate, pos);
				break;
			case 4: // imprimir lava	
				r_num = random() % 5;
				k = 103;
				init_pair(k, COLOR_BLACK, lava.pixels[r_num].color);         
				attron(COLOR_PAIR(k));
				mvprintw(i, j*2, "  ");
            	attroff(COLOR_PAIR(k));
				break;
			case 5: // imprimir relva	 
				k = 104;
				init_pair(k, grass.pixels[0].color, walk.pixels[0].color);         
				attron(COLOR_PAIR(k));
				mvprintw(i, j*2, "''" );
            	attroff(COLOR_PAIR(k));
				break;
			case 6: // imprimir flor 
				k = 105;
				init_pair(k, grass.pixels[1].color, walk.pixels[0].color);         
				attron(COLOR_PAIR(k));
				mvprintw(i, j*2, "**" );
            	attroff(COLOR_PAIR(k));
				break;
			case 7: // imprimir água profunda 
				pos.x = j;
				pos.y = i;
				draw_to_screen(water, pos);
				k = 106;
				init_pair(k, COLOR_BLACK, water.pixels[0].color);          
				attron(COLOR_PAIR(k));
				mvprintw(i, j*2, "  " );
            	attroff(COLOR_PAIR(k));
				break;
			case 8: // imprimir água margem 
				k = 107;
				init_pair(k, COLOR_BLACK, water.pixels[1].color);          
				attron(COLOR_PAIR(k));
				mvprintw(i, j*2, "  " );
            	attroff(COLOR_PAIR(k));
				break;
			case 9: // imprimir baú 
				pos.x = j;
				pos.y = i;
				draw_to_screen(chest, pos);
				break;
			case 10: // imprimir porta 
				pos.x = j;
				pos.y = i;
				draw_to_screen(door, pos);
				break;
			case 11: // imprimir chave 
				k = 110;
				init_pair(k, key.pixels[0].color, walk.pixels[0].color);          
				attron(COLOR_PAIR(k));
				mvprintw(i, j*2, "-o" );
            	attroff(COLOR_PAIR(k));
				break;
			case 12: // imprimir chão da sala enquanto fechada com um cor própria 
				k = 111;
				init_pair(k, COLOR_BLACK, walk.pixels[1].color);          
				attron(COLOR_PAIR(k));
				mvprintw(i, j*2, "  " );
            	attroff(COLOR_PAIR(k));
				break;
			default:
				break;
		}	
      }
    }
}