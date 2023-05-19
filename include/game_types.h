#ifndef GAME_TYPES
#define GAME_TYPES

#include "engine_types.h"

#define INVENTORY_SLOTS 25

// Mobs

typedef enum {
    STUPID = 0,
    COWARD = 1,
    INTELLIGENT = 2
} MobBehavior;

typedef struct mob {
    Vector2D position;
    Vector2D targetPosition;
    int health;
    MobBehavior mobBehavior;
    unsigned long timeSinceLastUpdate; // Em microsegundos
    int chasingPlayer;
    int pathStep;
    Vector2D *path;
    int attackDamage;
} Mob;

// Mapa
typedef struct map {
	int object;  //0:local onde pode andar|1:parede|2:passagem de nível|3:vazio|4:lava|5:relva|6:flor|7:água|8:margem|9:baú|10:porta|11:chave|12:chão sala fechada
    int visited; //0: às escuras, jogador nunca passou por lá, logo nunca foi iluminado | 1: está a ser ou já foi iluminado, neste caso se não estiver o jogador na zona fica a "sombreado"
} Map;

typedef struct bullet {
    int damage;
    Vector2D target;
    Vector2D position;
} Bullet;

// Mundo de Mapas
typedef struct world {
	Map** map;
    Mob *mobs;
    int mobQuantity;
    int created; // valida se um nível já foi gerado ou não
    Bullet *bullets;
    int bulletQuantity;
    int collectedChestItems;
} World;

// Items

typedef enum {
    NONE = 0,
    MELEE_WEAPON = 1,
    RANGED_WEAPON = 2,
    BOMB = 3,
    MISCELLANEOUS = 4,
    SPECIAL = 5,
    KEY = 6,
    WALK = 7
} ItemType;

typedef struct item {
    char name[100];
    ItemType type;
    char description[400];
    char sprite[200];
    int damage;
    unsigned long cooldown;
    int picked;
} Item;

typedef struct itemTable {
    int size;
    ItemType type;
    Item items[10];
} ItemsTable;

/*typedef struct {
    Item items[5];
    Vector2D position;
} Chest;*/

typedef struct inventory {
    Item items[INVENTORY_SLOTS];
} Inventory;

// Jogador

typedef struct player {
    char name[15];
    int health;
    Vector2D position;
    int gold;
    Inventory inventory;
    int selectedSlot;
    int speedMultiplier;
    unsigned long timeSinceDrownStart;
    unsigned long timeSinceLastAction;
} Player;

typedef struct pathBehaviour {
    Vector2D pathPos;
    Vector2D *path;
    int pathStep;
    int moving;
    unsigned long timeSinceLastUpdate; // Em microsegundos
} PathBehaviour;

// Estado do jogo

typedef struct gameState {
    Player player;
    int gameOver; // 1 - perdeu o jogo |  2 - ganhou o jogo
    int paused;
    int pathSelection;
    PathBehaviour pathState;
    int mobsInUI;
    unsigned long timeSinceLastBulletUpdate;
} GameState;


#endif