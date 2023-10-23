/////////////////////////////////////////////////////////////////////
// Carlos Hernandez
// All rights reserved
/////////////////////////////////////////////////////////////////////

#include "heap.h"
#include "bod.h"
#include "include.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <sys/resource.h>

gnode* graph_node;
unsigned num_gnodes;
// unsigned adjacent_table[MAXNODES][MAXNEIGH];
// unsigned pred_adjacent_table[MAXNODES][MAXNEIGH];
unsigned goal, start;
gnode* start_state;
gnode* goal_state;
snode* start_node;

unsigned long long int stat_expansions = 0;
unsigned long long int stat_generated = 0;
unsigned long long int minf_solution = LARGE;

// unsigned solutions[MAX_SOLUTIONS][2];

unsigned stat_pruned = 0;

unsigned **adjacent_table;
unsigned **pred_adjacent_table;


void allocateMemoryForTable(unsigned num_gnodes, unsigned num_arcs) {
    double ratio = (double)num_arcs / num_gnodes ;
    unsigned numNeighbors = (unsigned) round(ratio);
    if (numNeighbors<10){
        numNeighbors = 45;
    }
    else {
        numNeighbors = 3000*numNeighbors;
    }

    adjacent_table = malloc(num_gnodes * sizeof(unsigned *));
    pred_adjacent_table = malloc(num_gnodes * sizeof(unsigned *));
    if(adjacent_table == NULL || pred_adjacent_table == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    for (unsigned i = 0; i < num_gnodes; ++i) {
        adjacent_table[i] = malloc(numNeighbors * sizeof(unsigned));
        pred_adjacent_table[i] = malloc(numNeighbors * sizeof(unsigned));
        if(adjacent_table[i] == NULL || pred_adjacent_table[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
    }
}

void freeMemoryForTable(unsigned numNodes) {
    for (unsigned i = 0; i < numNodes; ++i) {
        free(adjacent_table[i]);
        free(pred_adjacent_table[i]);
    }
    free(adjacent_table);
    free(pred_adjacent_table);
}

void print_memory_usage_bod(const char* annotation) {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) != 0) {
        fprintf(stderr, "Could not get memory usage.\n");
        return;
    }
    printf("Memory usage at %s: %ld KB\n", annotation, usage.ru_maxrss);
}




void initialize_parameters() {
    start_state = &graph_node[start];
    goal_state = &graph_node[goal];
    stat_percolations = 0;
    
    for (int i = 0; i < num_gnodes; ++i){
        graph_node[i].gmin = LARGE;
    }
}


snode* new_node() {
    snode* state = (snode*)malloc(sizeof(snode));
//     state->visited_in_this_path = calloc(num_gnodes, sizeof(int)); // Initializes all elements to 0
    state->heapindex = 0;
    return state;
}

void free_node(snode* node) {
    if(node != NULL) { // always good to check if the pointer is not NULL
//         free(node->visited_in_this_path);
        free(node);
        node = NULL; // good practice to set freed pointer to NULL
    }
}

int bod(BodSolutions* s_array) {
    snode* recycled_nodes[MAX_RECYCLE];
    int next_recycled = 0;
    int nsolutions = 0;
    stat_pruned = 0;

    emptyheap();

    start_node = new_node();
    start_node->state = start;
    start_node->g1 = 0;
    start_node->g2 = 0;
    start_node->key = 0;
    start_node->searchtree = NULL;
//     start_node->visited_in_this_path[start] = 1;
    insertheap(start_node);

    stat_expansions = 0;
    
//     printf("start node: %d\n", start_node->state);

    while (topheap() != NULL) {
        snode* n = popheap(); //best node in open
        short d;

        if (n->g2 >= graph_node[n->state].gmin) {
            stat_pruned++;
//             if (next_recycled < MAX_RECYCLE) {
//                 recycled_nodes[next_recycled++] = n;
//             }
            free_node(n);
            continue;
        }

        graph_node[n->state].gmin = n->g2;
        
//         nsolutions = solutions[n->state].number_of_solution;
//         solutions[n->state].solution_vector[nsolutions]][0]= n->g1;
//         solutions[n->state].solution_vector[nsolutions]][1]= n->g2;
//         solutions[n->state].number_of_solution = nsolutions+1;
        
        SolutionNode* new_sol_node = malloc(sizeof(SolutionNode));
//         printf("Allocated SolutionNode at %p\n", (void*)new_sol_node);
        new_sol_node->solution[0] = n->g1;  // your solution values
        new_sol_node->solution[1] = n->g2;
        new_sol_node->next = s_array[n->state].head;
        s_array[n->state].head = new_sol_node;
        s_array[n->state].number_of_solution++;
        
        
        ++stat_expansions;
        

        for (d = 1; d < adjacent_table[n->state][0] * 3; d += 3) {
            snode* pred;
            double newkey;
            unsigned nsucc = adjacent_table[n->state][d];
            unsigned cost1 = adjacent_table[n->state][d + 1];
            unsigned cost2 = adjacent_table[n->state][d + 2];

            unsigned newg1 = n->g1 + cost1;
            unsigned newg2 = n->g2 + cost2;
            
            

            if (newg2 >= graph_node[nsucc].gmin)
                continue;
            
           pred = new_node();
            pred->state = nsucc;
            stat_generated++;

            newkey = newg1 * (double)BASE + newg2;
//             pred->searchtree = n;
            pred->g1 = newg1;
            pred->g2 = newg2;
            pred->key = newkey;
            
            insertheap(pred);
        }
        free_node(n);
    }
    int freeCnt;
    for(freeCnt=0;freeCnt < next_recycled;++freeCnt ){
        free_node(recycled_nodes[freeCnt]);
    }
    return 1;
    
}
/* ------------------------------------------------------------------------------*/
BodSolutions* call_bod() {
    float runtime;
    struct timeval tstart, tend;

    initialize_parameters();
    BodSolutions* solutions_array = malloc(num_gnodes * sizeof(BodSolutions));
    
    // Initialize each Solutions struct in the array
    for (unsigned i = 0; i < num_gnodes; ++i) {
        solutions_array[i].number_of_solution = 0;
        solutions_array[i].head = NULL;
    }
    
    gettimeofday(&tstart, NULL);

    //BOD
    bod(solutions_array);

    gettimeofday(&tend, NULL);
    runtime = 1.0 * (tend.tv_sec - tstart.tv_sec) + 1.0 * (tend.tv_usec - tstart.tv_usec) / 1000000.0;

//     printf("%lld;%f;%llu;%llu\n",
//         start_state->id + 1,
//         runtime * 1000,
//         stat_generated,
//         stat_expansions);
    return solutions_array;
}
