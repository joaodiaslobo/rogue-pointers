#ifndef INVENTORY
#define INVENTORY

#include "game_types.h"
#include "game.h"

void swap_items(Inventory *inventory, int pos1, int pos2);

int add_item(Inventory *inventory, Item *item);

void add_item_to_position(Inventory *inventory, Item *item, int pos);

void delete_item_at_position(Inventory *inventory, int pos);

void delete_key(Inventory *inventory);

int get_item_quantity(Inventory *inventory);

int get_key_quantity(Inventory *inventory);

int get_item_quantity_by_type(Inventory *inventory, ItemType type);

Item init_empty_item();

void add_table_to_inventory(Inventory *inventory, ItemsTable *table);

Inventory initialize_inventory();

int pick_random_item(Inventory *inventory);

int getRandomNumber(int min, int max);

int all_collected(Item globalItems[], int x);

int choose_item_freq(ItemType type);

#endif