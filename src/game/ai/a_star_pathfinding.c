#include "a_star_pathfinding.h"
#include "engine_types.h"
#include "game_types.h"
#include "math.h"
#include "float.h"
#include <stdlib.h>

#include "image.h"
#include "draw.h"
#include  "ncurses.h"

/* Isto é uma implementação do Algoritmo A* em C
   Algoritmo utilizado na movimentação dos mobs e pathfinding do jogador.
*/

void find_path(Node *startNode, Node *endNode){
    Node *currentNode = startNode;
    startNode->localGoal = 0.0f;
    startNode->globalGoal = distance_between_points(startNode->pos, endNode->pos);

    NodeStack untestedNodes;
    untestedNodes.elements = 1;
    untestedNodes.nodes = malloc(sizeof(Node) * 500);
    untestedNodes.nodes[0] = *startNode;

    while(untestedNodes.elements != 0){
        order_untested_nodes(&untestedNodes);

        while(untestedNodes.elements != 0 && untestedNodes.nodes[0].visited){
            pop_first(&untestedNodes);
        }

        if(untestedNodes.elements == 0){
            break;
        }

        currentNode = &untestedNodes.nodes[0];
        currentNode->visited = 1;

        for(int i = 0; i < currentNode->nNeighbours; i++){
            if(!currentNode->neighbours[i]->visited && !currentNode->neighbours[i]->obstacle){
                //untestedNodes.nodes = realloc(untestedNodes.nodes, sizeof(Node) * (untestedNodes.elements + 1));
                mvprintw(5,5,"%d %d", currentNode->neighbours[i]->pos.x, currentNode->neighbours[i]->pos.y);
                untestedNodes.nodes[untestedNodes.elements] = *currentNode->neighbours[i];
                untestedNodes.elements++;
            }

            float possibleLowGoal = currentNode->localGoal + distance_between_points(currentNode->pos, currentNode->neighbours[i]->pos);

            if(possibleLowGoal < currentNode->neighbours[i]->localGoal){
                currentNode->neighbours[i]->parent = currentNode;
                currentNode->neighbours[i]->localGoal = possibleLowGoal;

                currentNode->neighbours[i]->globalGoal = currentNode->neighbours[i]->localGoal + distance_between_points(currentNode->neighbours[i]->pos, endNode->pos);
            }
        }
    }
    //draw_path(&untestedNodes, endNode);
}

void pop_first(NodeStack *stack){
    stack->elements--;
    for(int i = 0; i < stack->elements; i++){
        stack->nodes[i] = stack->nodes[i+1];
    }
}

void order_untested_nodes(NodeStack *stack){
    for(int i = 0; i < stack->elements; i++){
        for(int j = i; j < stack->elements; j++){
            if(stack->nodes[i].globalGoal < stack->nodes[j].globalGoal){
                Node temp = stack->nodes[i];
                stack->nodes[i] = stack->nodes[j];
                stack->nodes[j] = temp;
            }
        }
    }
}

float distance_between_points(Vector2D a, Vector2D b){
    return sqrt( (a.x - b.x) * (a.x - b.x) +  (a.y - b.y) * (a.y - b.y) );
}

Node *map_to_node_system(MAP **map, int r, int c){
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
            if(map[y][x].object != 1 && map[y][x].object != 3){
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

    for (int y = 0; y < r; y++) {
        for (int x = 0; x < c; x++) {
            mvprintw(nodes[y * c + x].pos.y, nodes[y * c + x].pos.x * 2, "-%d", nodes[y * c + x].obstacle);
        }
    }

    return nodes;
}

void draw_path(NodeStack *nodes, Node *endNode){

    Image test = load_image_from_file("assets/sprites/gate.sprite");

    Node *curr = endNode;
    while(curr->parent != NULL){
        draw_to_screen(test, curr->pos);
        curr = curr->parent;
    }
}