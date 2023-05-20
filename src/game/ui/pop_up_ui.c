#include "pop_up_ui.h"
#include <ncurses.h>
#include <string.h>
#include "utils.h"
#include "engine_types.h"
#include "game.h"
#include "main_menu.h"

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
		    game(terminal);
            clear();
		    break;
        default:
            break; 
        }
    selection = main_menu(terminal);
    }

    init_game_state();
    
    return !option;
}