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
    Inventory game_inventory = {
        {
            {"Bread & Blood", MELEE_WEAPON, "Perfect for slicing through the soft, pillowy texture\nof freshly baked bread, but it is also a formidable weapon,\ncapable of drawing blood with just a flick of the wrist.\nThis knife is a contradiction,\na blend of the mundane and the macabre,\na tool for both nourishment and destruction.", "assets/sprites/inventory/items/breadandblood.sprite"},
            {"Kitty Cut", MELEE_WEAPON, "This knife is a whimsical yet practical tool\nthat exudes charm and playfulness.\nBut what sets this knife apart is\nthe intricate carving of a playful cat,\nwith its tail curled around the handle\nand its paws holding onto the blade.", "assets/sprites/inventory/items/kittycut.sprite"},
            {"Annabeth", MELEE_WEAPON, "Annabeth is a mace of formidable power and striking beauty.\nIt is a weapon that seems almost alive,\na part of the wielder's own body,\nand capable of delivering devastating strikes with ease.", " "},
            {"Spiky", MELEE_WEAPON, "Spiky is a mace of ferocious power and unrelenting brutality.\nThere is no finesse or grace to its movements,\nonly raw power and unbridled aggression.\nWielding Spiky may grant you victory on the battlefield,\nbut at what cost to your own soul?", " "},
            {"Excalibur", MELEE_WEAPON, "A legendary weapon of unparalleled power and mystique,\nsteeped in centuries of myth and folklore.\nLegend has it that only the truest of heart\ncan wield Excalibur,\nand that the sword possesses the power\nto vanquish any foe,\nno matter how fearsome.", "assets/sprites/inventory/items/excalibur.sprite"},
            {"Freyr", MELEE_WEAPON, "The sword Freyr is a weapon of mythic proportions,\nsaid to have been wielded by the Norse god of the same name.\nYou can't help but feel a sense of awe and reverence\nfor the incredible craftsmanship and divine power\nthat went into its creation.", "assets/sprites/inventory/items/freyr.sprite"},
            {"Lightsaber", MELEE_WEAPON, "The lightsaber is a weapon unlike any other,\na weapon of the Jedi and Sith\nthat emits a blade of pure energy,\nhumming with a distinctive, high-pitched sound\nthat echoes through the air.\nWith its sleek, futuristic design\nand unparalleled cutting power,\nthe lightsaber is a symbol of mastery, skill, and the force.", "assets/sprites/inventory/items/lightsaber.sprite"},
            {"God Glock", RANGED_WEAPON, "God Glock is a pistol of exceptional power\nand divine precision.\nSome even say that the weapon is divinely blessed,\nguided by a higher power that ensures\nits bullets always find their mark.", " "},
            {"Shy Sniper", RANGED_WEAPON, "A weapon that demands both precision and secrecy,\nwith mystical properties that makes it almost undetectable.\nWith Shy Sniper in your hand,\nyou will be a master of the art of stealth,\na true warrior who strikes from the shadows\nand disappears without a trace.", " "},
            {"Arabella", RANGED_WEAPON, "Arabella is a pistol of elegance and power,\na weapon that combines beauty with deadly efficiency.\nWith a classic, vintage design and exquisite details,\nthis pistol is a true work of art.\nStreaking hearts with a single shot will never be so intense.", " "},
            {"Blind Gas", BOMB, "Turns off the light.", " "},
            {"Defensive", BOMB, "Makes noise and damage.", " "},
            {"The Fireman's Worst Nightmare", BOMB, "Light, heat and fire: the arsonist.", " "},
            {"Flashbang", BOMB, "Flash and noise, dizziness and confusion.", " "},
            {"Nightstick", BOMB, "Enlightening your path for a moment.", " "}
        }
    };
    
    Image image = load_image_from_file("assets/sprites/inventory/title.sprite");
    Vector2D pos;
    pos.x = ((terminal->xMax - 144)/4); pos.y = 5;
    draw_to_window(inventoryWindow, image, pos);
    wrefresh(inventoryWindow);
    
    keypad(inventoryWindow, true);

    int selection = state->player.selectedSlot, key = 0, options = 15;

    while (key != 10){

        clear_item(terminal, inventoryWindow);
        box(inventoryWindow, 0, 0);
        
        for(int i = 0; i < options; i++){
            if(i == selection){

                wattron(inventoryWindow, A_BOLD);
                mvwprintw(inventoryWindow, i+(terminal->yMax / 2 - 3), (terminal->xMax / 2 - 10), "[x] %s", game_inventory.items[i].name);
                wattroff(inventoryWindow, A_BOLD);
                item_description(terminal, inventoryWindow, game_inventory.items[i].description);
                box(inventoryWindow, 0, 0);

                if(i == 0 || i == 1 || i == 4 || i == 5 || i == 6){ //por enquanto só há imagens para estes itens

                    Vector2D spritePos; spritePos.x = (terminal->xMax / 2 - 30); spritePos.y = (terminal->yMax / 2 - 4);
                    Image spriteImage = load_image_from_file(game_inventory.items[i].sprite);
                    draw_to_window(inventoryWindow, spriteImage, spritePos);
                }
            }
            else{
                mvwprintw(inventoryWindow, i+(terminal->yMax / 2 - 3), (terminal->xMax / 2 - 10), "[ ] %s", game_inventory.items[i].name);
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

    if(strlen(description) < 100){
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