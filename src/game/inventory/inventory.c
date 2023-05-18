#include <stdlib.h>
#include <time.h>
#include "inventory.h"
#include "global_items.h"
#include "game_types.h"
#include "game.h"

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

int pick_random_item(Inventory *inventory){
    
    srand(time(NULL));
    
    if(all_collected(globalItems, 9)){
        return -1;
    }
    else{
        int randomNumber = getRandomNumber(0, 9);
        if(globalItems[randomNumber].picked == 0){
            globalItems[randomNumber].picked = 1;
            return randomNumber;
        }
        else{
            pick_random_item(inventory);
        }
    }
}

int getRandomNumber(int min, int max){
    return min + rand() % (max - min + 1);
}

int all_collected(Item globalItems[], int x){
    for(int i = 0; i <= x; i++){
        if(globalItems[i].picked != 1){
            return 0;
        }
    }
    return 1;
}

int choose_item_freq(ItemType type){
    if(type == BOMB){
        return ((LEVEL % 4) + 1);
    }
    else if(type == MISCELLANEOUS){
        return ((LEVEL % 5) + 1);
    }
    else{
        return ((LEVEL % 2) + 1);
    }

    return 0;
}