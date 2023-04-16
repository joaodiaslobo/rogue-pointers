#ifndef HEADER_FILE
#define HEADER_FILE

#include "engine_types.h"

#define INVENTORY_SLOTS 40

// Items

typedef enum {
    MELEE_WEAPON = 0,
    RANGED_WEAPON = 1,
    BOMB = 2,
    MISCELLANEOUS = 3
} ItemType;

typedef struct {
    char name[20];
    ItemType type;
    char description[100];
} Item;

// Jogador

typedef struct {
    char name[15];
    Vector2D position;
    int gold;
    Item inventory[INVENTORY_SLOTS];
    int selectedSlot;
} Player;


#endif
