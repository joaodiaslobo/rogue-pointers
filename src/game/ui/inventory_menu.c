#include "inventory_menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "game_types.h"
#include "engine_types.h"
#include "global_items.h"
#include "inventory.h"
#include "image.h"
#include "draw.h"

void show_inventory(Terminal *terminal, WINDOW *inventoryWindow, GameState *state){
    Image image = load_image_from_file("assets/sprites/inventory/title.sprite");
    Vector2D pos;
    pos.x = ((terminal->xMax - 144)/4); pos.y = 5;
    draw_to_window(inventoryWindow, image, pos);
    wrefresh(inventoryWindow);
    
    keypad(inventoryWindow, true);

    int selection = state->player.selectedSlot, key = 0, options = get_item_quantity(&state->player.inventory);

    if(options == 0) options = 1;

    while (key != 10){

        clear_item(terminal, inventoryWindow);
        box(inventoryWindow, 0, 0);
        
        for(int i = 0; i < options; i++){
            if(state->player.inventory.items[i].type == NONE) continue;
                if(i == selection){
                    wattron(inventoryWindow, A_BOLD);
                    mvwprintw(inventoryWindow, i+(terminal->yMax / 2 - 3), (terminal->xMax / 2 - 10), "[x] %s", state->player.inventory.items[i].name);
                    wattroff(inventoryWindow, A_BOLD);
                    item_description(terminal, inventoryWindow, state->player.inventory.items[i].description);
                    box(inventoryWindow, 0, 0);
                    int fileNameSize = strlen(state->player.inventory.items[i].sprite);
                    if(fileNameSize > 1){
                        Vector2D spritePos; spritePos.x = (terminal->xMax / 2 - 30); spritePos.y = (terminal->yMax / 2 - 4);
                        Image spriteImage = load_image_from_file(state->player.inventory.items[i].sprite);
                        draw_to_window(inventoryWindow, spriteImage, spritePos);
                    }
                    if(state->player.inventory.items[i].type == MISCELLANEOUS || state->player.inventory.items[i].type == BOMB){
                        mvwprintw(inventoryWindow, (terminal->yMax / 2 + 20), (terminal->xMax / 2 + 37), "Quantity: %d", state->player.inventory.items[i].picked);
                    }
                }
            else{
                mvwprintw(inventoryWindow, i+(terminal->yMax / 2 - 3), (terminal->xMax / 2 - 10), "[ ] %s", state->player.inventory.items[i].name);
            }
        }

        key = wgetch(inventoryWindow);
        switch (key){
            
            case KEY_UP:
                if(selection > 0){
                    selection--;
                } else {
                    selection = options - 1;
                }
                break;
            case KEY_DOWN:
                if(selection < options - 1){
                    selection++;
                } else {
                    selection = 0;
                }
                break;
            case 10:
                state->player.selectedSlot = selection;
                break;
            default:
                break;
        }
    }
    //keypad(inventoryWindow, false);
}

void clear_item(Terminal *terminal, WINDOW *window){
    Vector2D pos;
    Image image = load_image_from_file("assets/sprites/shadow.sprite");

    for(int i = (terminal->xMax / 2 - 36); i < terminal->xMax; i++){
        for(int j = (terminal->yMax / 2 - 4); j < terminal->yMax; j++){
            
            pos.x = i; pos.y = j;
            draw_to_window(window, image, pos);
        }
    }
}

void item_description(Terminal *terminal, WINDOW *window, char description[]){
    int r = (terminal->yMax / 2 + 15);
    int c = (terminal->xMax / 2 + 37);

    if(strlen(description) < 50){
        r += 2;
        c += 14;
        mvwprintw(window, r, c, "%s", description);
    }

    else {
        int descriptionLength = strlen(description);
        for(int i = 0; i < descriptionLength; i++){
            
            if(description[i] != '\n'){
                mvwprintw(window, r, c, "%c", description[i]);
            }
            else{
                r++;
                c = (terminal->xMax / 2 + 37) - 1;
                wmove(window, r, c);
                mvwprintw(window, r, c, "%c", description[i]);
            }

            c++;
        }
    }
}