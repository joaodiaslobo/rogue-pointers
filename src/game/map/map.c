#include <string.h>
#include <ncurses.h>
// Necessário para as funções que geram o mapa
int NUM_COLUMNS;

// Mapa
typedef struct map {
	int object;  // 0 corresponde a local onde pode andar | 1 corresponde a uma parede | 2 corresponde a passagem de nível
} MAP;

void new_level_map (MAP (*a)[NUM_COLUMNS], int r, int c) {
	int random_num, count = 0, rand_cols, rand_rows;

	random_num = (random() % 3);
	while(count <= random_num) {
		rand_cols = (random() % c);
		rand_rows = (random() % r);
		if (a[rand_rows][rand_cols].object == 0) {
			a[rand_rows][rand_cols].object = 2;
			count++;
		}
	}
}

// Gera as borders do mapa
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

// Gera a primeira versão do mapa de uma forma aleatória
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

// Aliza o mapa
void smooth_map(MAP (*a)[NUM_COLUMNS], int r, int c, int x1, int x2) {
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

// Gera o mapa em 3 etaps
void gen_map(MAP (*a)[NUM_COLUMNS], int r, int c) {
   
   gen_border_map(a,r,c);
   gen_first_map(a,r,c);
   smooth_map(a,r,c,5,2);  
   new_level_map(a,r,c);
}

// Imprime o mapa
void print_map(MAP (*a)[NUM_COLUMNS], int r, int c) {
   Image wall = load_image_from_file("assets/sprites/wall.sprite");
   for (int i = 0; i < r; i++){
      for (int j = 0; j < c; j++){
		move(i,j);
		attron(COLOR_PAIR(COLOR_WHITE));
		//if (a[i][j].object == 0) printw(".");
		if (a[i][j].object == 0){
			
		}
		//else if (a[i][j].object == 1) printw("#");
		else if(a[i][j].object == 1){
			Vector2D pos = {j,i};
			draw_to_screen(wall, pos);
		}
		else if(a[i][j].object == 2){
			move(j,i);
			printw("X");
		}
		//attroff(COLOR_PAIR(a[i][j].color));
      }
    }
}
