#include "a_star_pathfinding.h"
#include "engine_types.h"
#include "game_types.h"
#include <math.h>
#include <float.h>
#include <stdlib.h>

#include "image.h"
#include "draw.h"
#include  <ncurses.h>
#include <string.h>

/* Isto é uma implementação do Algoritmo A* em C
   Algoritmo utilizado na movimentação dos mobs e pathfinding do jogador.
*/

Vector2D *find_path(Node *nodes, Vector2D start, Vector2D end, int c, int r, int *nSteps) {
    NodeStack openSet = { malloc(c * r * sizeof(Node *)), 0 };
    NodeStack closedSet = { malloc(c * r * sizeof(Node *)), 0 };
    Node *startNode = &nodes[start.y * c + start.x];
    Node *endNode = &nodes[end.y * c + end.x];
    startNode->globalGoal = heuristic(start, end);
    startNode->localGoal = 0.0f;
    openSet.nodes[openSet.elements] = startNode;
    openSet.elements++;
    while (openSet.elements > 0) {
        Node *currentNode = find_best_node(&openSet, &closedSet, end);
        // Handling de erro quando nodos são mal alocados nos sets
        if(currentNode == NULL){
            return NULL;
        }
        if (currentNode == endNode) {
            Vector2D *path = malloc((int)ceil(currentNode->localGoal) * sizeof(Vector2D));
            if(path == NULL){
                return NULL;
            }
            Node *node = currentNode;
            int steps = 0;
            for (int i = currentNode->localGoal - 1; i >= 0; i--) {
                path[i] = node->pos;
                node = node->parent;
                steps++;
            }
            free(openSet.nodes);
            free(closedSet.nodes);
            *nSteps = steps;
            return path;
        }
        currentNode->visited = 1;
        for (int i = 0; i < currentNode->nNeighbours; i++) {
            if(currentNode->neighbours[i] != NULL){
                Node *neighbourNode = currentNode->neighbours[i];
                if (neighbourNode->visited || neighbourNode->obstacle) {
                    continue;
                }
                float tryGoal = currentNode->localGoal + 1.0f;
                if (tryGoal < neighbourNode->localGoal) {
                    update_node(neighbourNode, currentNode, end);
                    if (!neighbourNode->visited) {
                        openSet.nodes[openSet.elements] = neighbourNode;
                        openSet.elements++;
                    }
                }
            }
        }
        closedSet.nodes[closedSet.elements++] = currentNode;
    }
    Vector2D *path = NULL;
    free(openSet.nodes);
    free(closedSet.nodes);
    return path;
}

Node *find_best_node(NodeStack *openSet, NodeStack *closedSet, Vector2D end) {
    int i, bestIndex = 0;
    float lowestScore = INFINITY;
    for (i = 0; i < openSet->elements; i++) {
        Node *node = openSet->nodes[i];
        if(node == NULL){
            return NULL;
        }
        float score = node->globalGoal + heuristic(node->pos, end);
        if (score < lowestScore) {
            lowestScore = score;
            bestIndex = i;
        }
    }
    Node *bestNode = openSet->nodes[bestIndex];
    for (i = 0; i < closedSet->elements; i++) {
        if (closedSet->nodes[i] == bestNode) { 
            openSet->nodes[bestIndex] = openSet->nodes[openSet->elements];
            openSet->elements--;
            return find_best_node(openSet, closedSet, end);
        }
    }
    openSet->elements--;
    openSet->nodes[bestIndex] = openSet->nodes[openSet->elements];
    return bestNode;
}

void update_node(Node *node, Node *parent, Vector2D end) {
    node->parent = parent;
    node->globalGoal = parent->globalGoal + heuristic(node->pos, end);
    node->localGoal = parent->localGoal + 1.0f;
}

float heuristic(Vector2D a, Vector2D b) {
    return sqrtf(powf(b.x - a.x, 2.0f) + powf(b.y - a.y, 2.0f));
}

Node *map_to_node_system(Map **map, int r, int c){
    Node *nodes = malloc(sizeof(Node) * r * c);

    // Criar nodos da grelha

    for (int y = 0; y < r; y++) {
        for (int x = 0; x < c; x++) {
            nodes[y * c + x].pos.x = x;
            nodes[y * c + x].pos.y = y;
            nodes[y * c + x].parent = NULL;
            nodes[y * c + x].visited = 0;
            nodes[y * c + x].globalGoal = FLT_MAX;
            nodes[y * c + x].localGoal = FLT_MAX;
            if(map[y][x].object != 1 && map[y][x].object != 3 && map[y][x].object != 10){
                nodes[y * c + x].obstacle = 0;
            } else {
                nodes[y * c + x].obstacle = 1;
            }
        }
    }

    // Fazer as conexões de vizinhança (3 é vazio)
    /* Distribuição dos pointers dos nodos na array de vizinhos:
        0   1   2
        3   x   4
        5   6   7
    */

    for (int y = 0; y < r; y++) {
        for (int x = 0; x < c; x++) {
            for(int i = 0; i < 8; i++){
                nodes[y * c + x].neighbours[i] = NULL;
            }
            int nNeighbours = 0;
            if(y != 0 && x != 0){
                nodes[y * c + x].neighbours[0] = &nodes[(y-1) * c + (x-1)];
                nNeighbours++;
            }
            if(y != 0){
                nodes[y * c + x].neighbours[1] = &nodes[(y-1) * c + x];
                nNeighbours++;
            }
            if(y != 0 && x != c - 1){
                nodes[y * c + x].neighbours[2] = &nodes[(y-1) * c + (x+1)];
                nNeighbours++;
            }
            if(x != 0){
                nodes[y * c + x].neighbours[3] = &nodes[y * c + (x-1)];
                nNeighbours++;
            }
            if(x != c - 1){
                nodes[y * c + x].neighbours[4] = &nodes[y * c + (x+1)];
                nNeighbours++;
            }
            if(y != r - 1 && x != 0){
                nodes[y * c + x].neighbours[5] = &nodes[(y+1) * c + (x-1)];
                nNeighbours++;
            }
            if(y != r - 1){
                nodes[y * c + x].neighbours[6] = &nodes[(y+1) * c + x];
                nNeighbours++;
            }
            if(y != r - 1 && x != c - 1){
                nodes[y * c + x].neighbours[7] = &nodes[(y+1) * c + (x+1)];
                nNeighbours++;
            }
            nodes[y * c + x].nNeighbours = nNeighbours;
        }
    }

    return nodes;
}