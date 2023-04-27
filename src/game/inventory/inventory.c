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