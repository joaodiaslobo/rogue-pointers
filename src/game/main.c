#include <ncurses.h>
#include "engine_types.h"
#include "color.h"
#include "main_menu.h"
#include "game.h"
#include <pthread.h>
#include "sound.h"

int main(){

    // Setup do ncurses
    WINDOW *window = initscr();

    // Certifica-se que o terminal onde o jogo está a rodar suporta a utilização de cores
    if(!can_change_color()){
        printw("Terminal não suporta cores.");
        getch();
        return -1;
    }
     
    // Faz com que o cursor fique invisível durante o jogo
    curs_set(0);

    cbreak();
	noecho();

    nodelay(stdscr, TRUE);
	meta(stdscr, TRUE);
	keypad(stdscr, TRUE);

    // NCurses passa a receber eventos do mouse 
    mousemask(BUTTON1_CLICKED | BUTTON3_CLICKED, NULL);

    //Coloca som no jogo
    SDL_Init(SDL_INIT_AUDIO); //inicializa o áudio da SDL
	pthread_t thread; // Cria uma thread para reproduzir o som do jogo
    Sound *fich = malloc(sizeof(Sound));
    fich->filename = "assets/sound/game.wav";
    fich->time_ms = 168000;
	fich->loop = 1;
    if (pthread_create(&thread, NULL, play_sound_thread, fich) != 0)  {
        printw("Erro ao criar a thread\n");
    }


    // Obtem tamanho do terminal
    Terminal terminal;
    getmaxyx(stdscr, terminal.yMax, terminal.xMax);
	terminal.mainWindow = window;
    terminal.loadedRuntimePairs = 0;

    start_color();

    // Carrega a palette para o ncurses
    if(!load_palette_from_file("assets/main.palette", &terminal)){
        printw("Falha no carregamento da palette.");
    }

    int selection = main_menu(&terminal);
    while(selection == 0 || selection == 3){ // permite que o jogador volte ao menu principal, caso perca
        switch (selection){
	    case 0:
		    game(&terminal);
            clear();
		    break;
        default:
            break; 
        }
    selection = main_menu(&terminal);
    }

    // Finaliza o ncurses
    endwin();
    return 0;
}
