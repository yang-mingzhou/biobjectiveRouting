#ifndef PATHRETRIEVAL_H
#define PATHRETRIEVAL_H

#define MAX_SOLUTIONS 1000000
#include "graph_data.h"
#ifdef __cplusplus
extern "C" {
#endif
    
    
// void read_adjacent_table(const char* filename, unsigned & numOfGnode,  unsigned input_adjacent_table[MAXNODES][MAXNEIGH], unsigned input_pred_adjacent_table[MAXNODES][MAXNEIGH]);
    

void initializeGraphDataBOA(GraphData* graphData, int num_nodes, int num_arcs);
void cleanupGraphData(GraphData* graphData) ;
void printEdgeVectors(const GraphData* graphData);

void assign_global_variables(const GraphData* graphData);    

unsigned (*paretoPathsInFragment(int s_node, int d_node, const GraphData* graphData))[2];
// unsigned (*namorInFragment(int s_node, int d_node, const GraphData* graphData))[2];
// unsigned (*paretoPathsInFragmentChar(int s_node, int d_node, const char* filename))[2];
    

// void myCFunction(struct GraphData* graphData);

#ifdef __cplusplus
}
#endif

#endif