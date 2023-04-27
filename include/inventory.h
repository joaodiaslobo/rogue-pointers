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

#endif