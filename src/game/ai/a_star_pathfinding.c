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

Vector2D *find_path(Node *nodes, Vector2D start, Vector2D end, int c){
    Image test = load_image_from_file("assets/sprites/gate.sprite");
    Node *currentNode = &nodes[start.y * c + start.x];
    nodes[start.y * c + start.x].localGoal = 0.0f;
    nodes[start.y * c + start.x].globalGoal = distance_between_points(start, end);

    NodeStack untestedNodes;
    untestedNodes.elements = 1;
    untestedNodes.nodes = malloc(sizeof(Node *));
    untestedNodes.nodes[0] = currentNode;

    while(untestedNodes.elements != 0){
        order_untested_nodes(&untestedNodes);

        while(untestedNodes.elements != 0 && untestedNodes.nodes[0]->visited){
            pop_first(&untestedNodes);
        }

        if(untestedNodes.elements == 0){
            break;
        }

        currentNode = untestedNodes.nodes[0];
        currentNode->visited = 1;

        for(int i = 0; i < currentNode->nNeighbours; i++){
            if(currentNode->neighbours[i] != NULL){
                if(!currentNode->neighbours[i]->visited && !currentNode->neighbours[i]->obstacle){
                    untestedNodes.nodes = realloc(untestedNodes.nodes, sizeof(Node *) * (untestedNodes.elements + 1));
                    untestedNodes.nodes[untestedNodes.elements] = currentNode->neighbours[i];
                    untestedNodes.elements++;
                }
            }

            float possibleLowGoal = currentNode->localGoal + distance_between_points(currentNode->pos, currentNode->neighbours[i]->pos);

            if(possibleLowGoal < currentNode->neighbours[i]->localGoal){
                currentNode->neighbours[i]->parent = currentNode;
                currentNode->neighbours[i]->localGoal = possibleLowGoal;

                currentNode->neighbours[i]->globalGoal = currentNode->neighbours[i]->localGoal + distance_between_points(currentNode->neighbours[i]->pos, end);
            }
        }
    }

    Vector2D *path = malloc(sizeof(Vector2D));
    currentNode = &nodes[end.y * c + end.x];
    int i = 0;
    while(currentNode->parent != NULL){
        path = realloc(path, (i + 1) * sizeof(Vector2D));
        Vector2D pos = { currentNode->pos.x, currentNode->pos.y };
        path[i] = pos;
        currentNode = currentNode->parent;
        i++;
    }
    

    return path;
}

void inspect_node(Node *nodes, Vector2D pos, int c){
    Node *currentNode = &nodes[pos.y * c + pos.x];
    mvprintw(0,0, "X: %d", nodes[pos.y * c + pos.x].pos.x);
    mvprintw(1,0, "Y: %d", nodes[pos.y * c + pos.x].pos.y);
    mvprintw(2,0, "PARENT: %p", nodes[pos.y * c + pos.x].parent);
    mvprintw(3,0, "VIZINHOS: %d", nodes[pos.y * c + pos.x].nNeighbours);
    if(currentNode->neighbours[0] != NULL){
        mvprintw(4,0, "%d", currentNode->neighbours[0]->obstacle);
    }
    if(currentNode->neighbours[1] != NULL){
        mvprintw(4,2, "%d", currentNode->neighbours[1]->obstacle);
    }
    if(currentNode->neighbours[2] != NULL){
        mvprintw(4,4, "%d", currentNode->neighbours[2]->obstacle);
    }
    if(currentNode->neighbours[3] != NULL){
        mvprintw(5,0, "%d", currentNode->neighbours[3]->obstacle);
    }
    if(currentNode->neighbours[4] != NULL){
        mvprintw(5,4, "%d", currentNode->neighbours[4]->obstacle);
    }
    if(currentNode->neighbours[5] != NULL){
        mvprintw(6,0, "%d", currentNode->neighbours[5]->obstacle);
    }
    if(currentNode->neighbours[6] != NULL){
        mvprintw(6,2, "%d", currentNode->neighbours[6]->obstacle);
    }
    if(currentNode->neighbours[7] != NULL){
        mvprintw(6,4, "%d", currentNode->neighbours[7]->obstacle);
    }
}

void pop_first(NodeStack *stack){
    stack->nodes[0] = NULL;
    stack->elements--;
    memmove(stack->nodes, stack->nodes + 1, (sizeof(Node *) * (stack->elements)));
}

void order_untested_nodes(NodeStack *stack){
    for(int i = 0; i < stack->elements; i++){
        for(int j = i; j < stack->elements; j++){
            if(stack->nodes[i]->globalGoal < stack->nodes[j]->globalGoal){
                Node *temp = stack->nodes[i];
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
            nodes[y * c + x].neighbours[0] = NULL;
            nodes[y * c + x].neighbours[1] = NULL;
            nodes[y * c + x].neighbours[2] = NULL;
            nodes[y * c + x].neighbours[3] = NULL;
            nodes[y * c + x].neighbours[4] = NULL;
            nodes[y * c + x].neighbours[5] = NULL;
            nodes[y * c + x].neighbours[6] = NULL;
            nodes[y * c + x].neighbours[7] = NULL;
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

void draw_path(NodeStack *nodes, Node *endNode){

    Image test = load_image_from_file("assets/sprites/gate.sprite");

    Node *curr = endNode;
    while(curr->parent != NULL){
        draw_to_screen(test, curr->pos);
        curr = curr->parent;
    }
}