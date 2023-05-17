#ifndef INVENTORY
#define INVENTORY

#include "game_types.h"

void swap_items(Inventory *inventory, int pos1, int pos2);

int add_item(Inventory *inventory, Item *item);

void add_item_to_position(Inventory *inventory, Item *item, int pos);

void delete_item_at_position(Inventory *inventory, int pos);

int get_item_quantity(Inventory *inventory);

int get_item_quantity_by_type(Inventory *inventory, ItemType type);

Item init_empty_item();

void add_table_to_inventory(Inventory *inventory, ItemsTable *table);

Inventory initialize_inventory();

int pick_random_item(Inventory *inventory, int picked[], int level);

int getRandomNumber(int min, int max);

int all_collected(int picked[], int x);

int choose_item_freq(ItemType type, int level);

#endif