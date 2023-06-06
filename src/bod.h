#ifndef BOASTARH
#define BOASTARH

#define MAX_SOLUTIONS 1000000
#define MAX_RECYCLE   1000000
#include "node.h"
#include "graph_data.h"
extern gnode *graph_node;
extern unsigned num_gnodes;
// extern unsigned adjacent_table[MAXNODES][MAXNEIGH];
// extern unsigned pred_adjacent_table[MAXNODES][MAXNEIGH];
extern unsigned goal, start; 
extern unsigned **adjacent_table;
extern unsigned **pred_adjacent_table;

void allocateMemoryForTable(unsigned num_gnodes, unsigned num_arcs);
void freeMemoryForTable(unsigned numNodes);

typedef struct solution_node {
    int solution[2];
    struct solution_node* next;
} SolutionNode;

typedef struct {
    int number_of_solution;
    SolutionNode* head;
} BodSolutions;

BodSolutions* call_bod();

#endif
