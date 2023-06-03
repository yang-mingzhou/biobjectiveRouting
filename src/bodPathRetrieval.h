#ifndef PATHRETRIEVAL_H
#define PATHRETRIEVAL_H

#define MAX_SOLUTIONS 1000000

#ifdef __cplusplus
extern "C" {
#endif
    
    
// void read_adjacent_table(const char* filename, unsigned & numOfGnode,  unsigned input_adjacent_table[MAXNODES][MAXNEIGH], unsigned input_pred_adjacent_table[MAXNODES][MAXNEIGH]);
#ifndef GraphData
#define GraphData    
struct GraphData {
    int** edgeVectors;
    int numOfGnode;
    int numOfArcs;
};
#endif

void bod_initializeGraphData(struct GraphData* graphData, int num_nodes, int num_arcs);
void bod_cleanupGraphData(struct GraphData* graphData) ;
void bod_printEdgeVectors(const struct GraphData* graphData);

void bod_assign_global_variables(const struct GraphData* graphData);    

unsigned (*bod_paretoPathsInFragment(int s_node,  const struct GraphData* graphData))[2];
unsigned (*bod_paretoPathsInFragmentChar(int s_node, const char* filename))[2];
    

// void myCFunction(struct GraphData* graphData);

#ifdef __cplusplus
}
#endif

#endif