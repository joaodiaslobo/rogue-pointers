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

typedef struct {
    Node **nodes;
    int elements;
} NodeStack;

Vector2D *find_path(Node *nodes, Vector2D start, Vector2D end, int c);

void pop_first(NodeStack *stack);

void order_untested_nodes(NodeStack *stack);

float distance_between_points(Vector2D a, Vector2D b);

void inspect_node(Node *nodes, Vector2D pos, int c);

Node *map_to_node_system(MAP **map, int r, int c);

void draw_path(NodeStack *nodes, Node *endNode);

#endif