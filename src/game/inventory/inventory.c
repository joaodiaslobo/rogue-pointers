#include <stdlib.h>
#include <time.h>
#include "inventory.h"
#include "game_types.h"

void swap_items(Inventory *inventory, int pos1, int pos2){
    Item temp = inventory->items[pos1];
    inventory->items[pos1] = inventory->items[pos2];
    inventory->items[pos2] = temp;
}

int add_item(Inventory *inventory, Item *item){
    for(int i =  0; i < INVENTORY_SLOTS; i++){
        if (inventory->items[i].type == NONE){
            inventory->items[i] = *item;
            return 0;
        }
    }
    return 1;
}

void add_item_to_position(Inventory *inventory, Item *item, int pos){
    inventory->items[pos] = *item;
}

void delete_item_at_position(Inventory *inventory, int pos){
    inventory->items[pos] = init_empty_item();
}

int get_item_quantity(Inventory *inventory){
    int quantity = 0;
    for(int i = 0; i < INVENTORY_SLOTS; i++){
        if(inventory->items[i].type != NONE){
            quantity++;
        }
    }
    return quantity;
}

int get_item_quantity_by_type(Inventory *inventory, ItemType type){
    int quantity = 0;
    for(int i = 0; i < INVENTORY_SLOTS; i++){
        if(inventory->items[i].type == type){
            quantity++;
        }
    }
    return quantity;
}

Item init_empty_item(){
    Item item;
    item.type = NONE;
    return item;
}

void add_table_to_inventory(Inventory *inventory, ItemsTable *table){
    for(int i = 0; i < table->size; i++){
        add_item(inventory, &table->items[i]);
    }
}

Inventory initialize_inventory(){
    Inventory inventory;
    for(int i = 0; i < INVENTORY_SLOTS; i++){
        inventory.items[i].type = NONE;
    }
    return inventory;
}

int pick_random_item(Inventory *inventory, int picked[], int level){
    srand(time(NULL));
    //níveis 0 a 20:
    // de 0 a 4, (knifes: 0 e 1) MELEE_WEAPONS e 1 tipo de BOMB possível, freq <=2 (depois escolher a ordem de importância para as bombs), itens especiais não são possíveis
    // de 5 a 8, (knifes & maces: 0 a 3) MELEE WEAPONS e 2 tipos de BOMB possíveis, freq <= 5, itens especiais básicos (ordenar poções por importância)
    // de 9 a 12, (knifes, maces & swords: 0 a 6) MELEE WEAPONS e 3 tipos de BOMB possíveis, freq <= 8, itens especiais intermédios
    // de 13 a 16, introduzem-se as RANGED_WEAPONS (até 9) e 4 tipos de BOMB, freq <= 12, itens especiais intermédios mas mais freq (2 ou mais)
    // de 17 a 20, 5 tipos de BOMB, freq <= 15 e itens especiais altos
    // basicamente nas bombs era fixe desbloquear de 4 em 4 níveis, tipo avisava de propósito com pop up; deve dar com %4 == 0, algo assim
    if(level < 5){
        if(all_collected(picked, 1)){
            return -1;
        }
        else{
            int randomNumber = getRandomNumber(0, 1);
            if(picked[randomNumber] == 0){
                picked[randomNumber] = 1;
                return randomNumber;
            }
            else{
                pick_random_item(inventory, picked, level);
            }
        }
    }
    else if(level > 4 && level < 9){
        if(all_collected(picked, 3)){
            return -1;
        }
        else{
            int randomNumber = getRandomNumber(0, 3);
            if(picked[randomNumber] == 0){
                picked[randomNumber] = 1;
                return randomNumber;
            }
            else{
                pick_random_item(inventory, picked, level);
            }
        }
    }
    else if(level > 8 && level < 13){
        if(all_collected(picked, 6)){
            return -1;
        }
        else{
            int randomNumber = getRandomNumber(0, 6);
            if(picked[randomNumber] == 0){
                picked[randomNumber] = 1;
                return randomNumber;
            }
            else{
                pick_random_item(inventory, picked, level);
            }
        }
    }
    else if(level > 12 && level < 21){
        if(all_collected(picked, 9)){
            return -1;
        }
        else{
            int randomNumber = getRandomNumber(0, 9);
            if(picked[randomNumber] == 0){
                picked[randomNumber] = 1;
                return randomNumber;
            }
            else{
                pick_random_item(inventory, picked, level);
            }
        }
    }

    return 0;
}

int getRandomNumber(int min, int max){
    return min + rand() % (max - min + 1);
}

int all_collected(int picked[], int x){
    int n = 0;
    for(int i = 0; i <= x; i++){
        if(picked[i] != 0){
            n++;
        }
    }
    if(n == x){
        return 1;
    }
    else{
        return 0;
    }
}

int choose_item_freq(ItemType type, int level){ //BOMBS == %5+1, SPECIAL == %4+1
    if(type == BOMB){
        return ((level % 5) + 1);
    }
    else if(type == MISCELLANEOUS){
        return ((level % 4) + 1);
    }

    return 0;
}