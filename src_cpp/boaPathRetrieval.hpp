#ifndef PATHRETRIEVAL_H
#define PATHRETRIEVAL_H

#define MAX_SOLUTIONS 1000000

#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

struct GraphData {
    std::vector<int> edgeVectors;
    int numOfGnode;
    int numOfArcs;
};

void initializeGraphData(struct GraphData* graphData, int num_nodes, int num_arcs);
void cleanupGraphData(struct GraphData* graphData);
void printEdgeVectors(const struct GraphData* graphData);
void assign_global_variables(const struct GraphData* graphData);
unsigned (*paretoPathsInFragment(int s_node, int d_node, const struct GraphData* graphData))[2];
unsigned (*paretoPathsInFragmentChar(int s_node, int d_node, const char* filename))[2];

#ifdef __cplusplus
}
#endif

#endif
