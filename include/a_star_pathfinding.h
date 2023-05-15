#ifndef A_STAR_PATHFINDING
#define A_STAR_PATHFINDING

#include "engine_types.h"
#include "game_types.h"

typedef struct node {
    int obstacle;
    int visited;
    float globalGoal;
    float localGoal;
    Vector2D pos;
    struct node *neighbours[8];
    int nNeighbours;
    struct node *parent;
} Node;

typedef struct nodeStack {
    Node **nodes;
    int elements;
} NodeStack;

Vector2D *find_path(Node *nodes, Vector2D start, Vector2D end, int c, int r, int *nSteps);

Node *find_best_node(NodeStack *open_set, NodeStack *closed_set, Vector2D end);

float heuristic(Vector2D a, Vector2D b);

void update_node(Node *node, Node *parent, Vector2D end);

Node *map_to_node_system(Map **map, int r, int c);

#endif