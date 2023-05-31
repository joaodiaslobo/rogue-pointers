#include <ncurses.h>
#include "engine_types.h"
#include "color.h"
#include "main_menu.h"
#include "game.h"
#include <pthread.h>
#include "sound.h"
#include "components.h"
#include "about_menu.h"

/*

* a104179, a104356, a104439 - Sara Lopes, João Lobo, Rita Camacho

* Entrypoint da aplicação. Inicia o ncurses e SDL2.

*/
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

    init_color(0, 0, 0, 0);

    int selection = main_menu(&terminal);
    while(selection == 0 || selection == 1 || selection == 3){ // permite que o jogador volte ao menu principal, caso perca
        switch (selection){
	    case 0:
            {
                Vector2D inputPos = {terminal.xMax / 2 - 15, terminal.yMax / 2};
                char *playerName = malloc(sizeof(char) * 16);
                text_input_box(inputPos, 30, 15, "Insert your name:", playerName);
                clear();
                refresh();
                game(&terminal, playerName);
                clear();
            }
		    break;
        case 1:
            {   
                WINDOW * aboutWindow = newwin(terminal.yMax, terminal.xMax, 0, 0);
                box(aboutWindow, 0, 0); 
                clear();
                refresh();
                show_about_menu(&terminal, aboutWindow);
                wrefresh(aboutWindow);
                delwin(aboutWindow);
                clear();
                refresh();
            }
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
