#ifndef BOASTARH
#define BOASTARH

#define MAX_SOLUTIONS 1000000
#define MAX_RECYCLE   1000000

extern gnode *graph_node;
extern unsigned num_gnodes;
extern unsigned adjacent_table[MAXNODES][MAXNEIGH];
extern unsigned pred_adjacent_table[MAXNODES][MAXNEIGH];
extern unsigned goal, start; 

typedef struct {
    unsigned number_of_solution;
    unsigned solution_vector[MAX_SOLUTIONS][2];
} Solutions;

Solutions* call_bod();

#endif
