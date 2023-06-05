#ifndef BODPATHRETRIEVAL_H
#define BODPATHRETRIEVAL_H
#include "bod.h"
#include "graph_data.h"
#define MAX_SOLUTIONS 1000000

#ifdef __cplusplus
extern "C" {
#endif
    
    
// void read_adjacent_table(const char* filename, unsigned & numOfGnode,  unsigned input_adjacent_table[MAXNODES][MAXNEIGH], unsigned input_pred_adjacent_table[MAXNODES][MAXNEIGH]);   


void bod_initializeGraphData(GraphData* graphData, int num_nodes, int num_arcs);
void bod_cleanupGraphData(GraphData* graphData) ;
void bod_printEdgeVectors(const GraphData* graphData);
void bod_freeSolutions(BodSolutions* solutions);
void bod_assign_global_variables(const GraphData* graphData);    

BodSolutions* bod_paretoPathsInFragment(int s_node,  const GraphData* graphData);
BodSolutions* bod_paretoPathsInFragmentChar(int s_node, const char* filename);
    

// void myCFunction(struct GraphData* graphData);

#ifdef __cplusplus
}
#endif

#endif