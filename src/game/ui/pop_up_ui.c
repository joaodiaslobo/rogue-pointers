#include "pop_up_ui.h"
#include <ncurses.h>
#include <string.h>
#include "utils.h"
#include "engine_types.h"
#include "game.h"
#include "main_menu.h"
#include <stdlib.h>
#include "components.h"

int new_items_pop_up(char text[], int width, int screenYMax, int screenXMax){

    int key = 0, option = 0;
    int necessaryLines = count_newlines(text) + 1 + 4 + 1;
    int posY = (screenYMax - necessaryLines) / 2;
    int posX = (screenXMax - width) / 2;

    WINDOW * newItemsWindow = newwin(necessaryLines, width, posY, posX);
    box(newItemsWindow, 0, 0);
    refresh();
    wrefresh(newItemsWindow);
    keypad(newItemsWindow, true);

    mvwprintw(newItemsWindow, 1, 2, "%s", text);
    
    if(LEVEL % 5 == 0){
        mvwprintw(newItemsWindow, 2, 2, "New bomb unlocked!");
    }

    while(key != 10){
        if(!option){
            wattron(newItemsWindow, A_BOLD);
            mvwprintw(newItemsWindow, necessaryLines - 2, width - 5, "OK!");
            wattroff(newItemsWindow, A_BOLD);
        }

        key = wgetch(newItemsWindow);
        switch (key)
        {
            case KEY_UP:
                if(option){
                    option--;
                } else {
                    option++;
                }
                break;
            case KEY_DOWN:
                if(option){
                    option--;
                } else {
                    option++;
                }
                break;
            default:
                break;
        }
    }

    wclear(newItemsWindow);
    wrefresh(newItemsWindow);
    delwin(newItemsWindow);
    return !option;
}

int winner_pop_up(char text[], int width, int screenYMax, int screenXMax, Terminal *terminal){

    int key = 0, option = 0;
    int necessaryLines = count_newlines(text) + 1 + 4 + 1;
    int posY = (screenYMax - necessaryLines) / 2;
    int posX = (screenXMax - width) / 2;

    WINDOW * winnerWindow = newwin(necessaryLines, width, posY, posX);
    box(winnerWindow, 0, 0);
    refresh();
    wrefresh(winnerWindow);
    keypad(winnerWindow, true);

    mvwprintw(winnerWindow, 1, 2, "%s", text);

    while(key != 10){
        
        wattron(winnerWindow, A_BOLD);
        mvwprintw(winnerWindow, necessaryLines - 2, width - 23, "YAY! START A NEW GAME");
        wattroff(winnerWindow, A_BOLD);

        key = wgetch(winnerWindow);
        switch (key)
        {
            case KEY_UP:
                if(option){
                    option--;
                } else {
                    option++;
                }
                break;
            case KEY_DOWN:
                if(option){
                    option--;
                } else {
                    option++;
                }
                break;
            default:
                break;
        }
    }

    wclear(winnerWindow);
    wrefresh(winnerWindow);
    delwin(winnerWindow);
    endwin();

    clear();
    refresh();
    int selection = main_menu(terminal);

    while(selection == 0 || selection == 3){
        switch (selection){
	    case 0:
            {
                Vector2D inputPos = {terminal->xMax / 2 - 15, terminal->yMax / 2};
                char *playerName = malloc(sizeof(char) * 16);
                text_input_box(inputPos, 30, 15, "Insert your name:", playerName);
                clear();
                refresh();
                game(terminal, playerName);
                clear();
            }
		    break;
        default:
            break; 
        }
    selection = main_menu(terminal);
    }

    init_game_state();
    
    return !option;
}

int game_over_pop_up(char text[], int width, int screenYMax, int screenXMax, Terminal *terminal){

    int key = 0, option = 0;
    int necessaryLines = count_newlines(text) + 1 + 4 + 1;
    int posY = (screenYMax - necessaryLines) / 2;
    int posX = (screenXMax - width) / 2;

    WINDOW * gameOverWindow = newwin(necessaryLines, width, posY, posX);
    box(gameOverWindow, 0, 0);
    refresh();
    wrefresh(gameOverWindow);
    keypad(gameOverWindow, true);

    mvwprintw(gameOverWindow, 1, 2, "%s", text);

    while(key != 10){
        
        wattron(gameOverWindow, A_BOLD);
        mvwprintw(gameOverWindow, necessaryLines - 2, width - 12, "TRY AGAIN!");
        wattroff(gameOverWindow, A_BOLD);

        key = wgetch(gameOverWindow);
        switch (key)
        {
            case KEY_UP:
                if(option){
                    option--;
                } else {
                    option++;
                }
                break;
            case KEY_DOWN:
                if(option){
                    option--;
                } else {
                    option++;
                }
                break;
            default:
                break;
        }
    }

    wclear(gameOverWindow);
    wrefresh(gameOverWindow);
    delwin(gameOverWindow);
    endwin();

    clear();
    refresh();
    int selection = main_menu(terminal);

    while(selection == 0 || selection == 3){
        switch (selection){
        case 0:
            {
                Vector2D inputPos = {terminal->xMax / 2 - 15, terminal->yMax / 2};
                char *playerName = malloc(sizeof(char) * 16);
                text_input_box(inputPos, 30, 15, "Insert your name:", playerName);
                clear();
                refresh();
                game(terminal, playerName);
                clear();
            }
            break;
        default:
            break; 
        }
    selection = main_menu(terminal);
    }

    init_game_state();

    return !option;
}

int pause_pop_up(char text[], int width, int screenYMax, int screenXMax, Terminal *terminal){

    int key = 0, option = 0;
    int necessaryLines = count_newlines(text) + 1 + 4 + 1;
    int posY = (screenYMax - necessaryLines) / 2;
    int posX = (screenXMax - width) / 2;

    WINDOW * pauseWindow = newwin(necessaryLines, width, posY, posX);
    box(pauseWindow, 0, 0);
    refresh();
    wrefresh(pauseWindow);
    keypad(pauseWindow, true);

    mvwprintw(pauseWindow, 1, 2, "%s", text);

    while(key != 10){
        if(!option){
            wattron(pauseWindow, A_BOLD);
            mvwprintw(pauseWindow, necessaryLines - 2, 2, "CONTINUE");
            wattroff(pauseWindow, A_BOLD);
            mvwprintw(pauseWindow, necessaryLines - 2, width - 9, "RESTART");
        } else {
            wattron(pauseWindow, A_BOLD);
            mvwprintw(pauseWindow, necessaryLines - 2, width - 9, "RESTART");
            wattroff(pauseWindow, A_BOLD);
            mvwprintw(pauseWindow, necessaryLines - 2, 2, "CONTINUE");
        }

        key = wgetch(pauseWindow);
        switch (key)
        {
            case KEY_RIGHT:
                if(option){
                    option--;
                } else {
                    option++;
                }
                break;
            case KEY_LEFT:
                if(option){
                    option--;
                } else {
                    option++;
                }
                break;
            case 10:
                if(option){
                    wclear(pauseWindow);
                    wrefresh(pauseWindow);
                    delwin(pauseWindow);
                    endwin();

                    clear();
                    refresh();
                    int selection = main_menu(terminal);

                    while(selection == 0 || selection == 3){
                        switch (selection){
                        case 0:
                            {
                                Vector2D inputPos = {terminal->xMax / 2 - 15, terminal->yMax / 2};
                                char *playerName = malloc(sizeof(char) * 16);
                                text_input_box(inputPos, 30, 15, "Insert your name:", playerName);
                                clear();
                                refresh();
                                game(terminal, playerName);
                                clear();
                            }
                            break;
                        default:
                            break; 
                        }
                    selection = main_menu(terminal);
                    }
                    init_game_state();
                }
                break;
            default:
                break;
        }
    }

    wclear(pauseWindow);
    wrefresh(pauseWindow);
    delwin(pauseWindow);
    endwin();

    return !option;
}