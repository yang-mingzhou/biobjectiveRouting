#ifndef NAMOASTARH
#define NAMOASTARH

#define MAX_SOLUTIONS 1000000
#define MAX_RECYCLE   1000000
#include "graph_data.h"
extern gnode *graph_node;
extern unsigned num_gnodes;
// extern unsigned adjacent_table[MAXNODES][MAXNEIGH];
// extern unsigned pred_adjacent_table[MAXNODES][MAXNEIGH];
extern unsigned **adjacent_table;
extern unsigned **pred_adjacent_table;
extern unsigned goal, start; 
void allocateMemoryForTable(unsigned num_gnodes, unsigned num_arcs);
void freeMemoryForTable(unsigned numNodes);

// extern unsigned goal, start; 
unsigned (*call_boastar())[2];
unsigned (*call_namoadr())[2];

#endif
