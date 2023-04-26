#ifndef GAME_TYPES
#define GAME_TYPES

#include "engine_types.h"

#define INVENTORY_SLOTS 40

// Mapa
typedef struct {
	int object;  // 0: local onde pode andar | 1: parede | 2: passagem de nível | 3: vazio
} Map;

// Items

typedef enum {
    NONE = 0,
    MELEE_WEAPON = 1,
    RANGED_WEAPON = 2,
    BOMB = 3,
    MISCELLANEOUS = 4
} ItemType;

typedef struct {
    char name[20];
    ItemType type;
    char description[100];
} Item;

typedef struct {
    Item items[INVENTORY_SLOTS];
} Inventory;

// Jogador

typedef struct {
    char name[15];
    Vector2D position;
    int gold;
    Inventory inventory;
    int selectedSlot;
    int speedMultiplier;
} Player;

// Estado do jogo

typedef struct {
    Player player;
} GameState;


#endif
