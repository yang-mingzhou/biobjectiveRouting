#include "boaPathRetrieval.hpp"
#include "include.hpp"
#include "boastar.hpp"
#include "graph.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

void initializeGraphData(struct GraphData* graphData, int num_nodes, int num_arcs) {
    graphData->numOfGnode = num_nodes;
    graphData->numOfArcs = num_arcs;

    graphData->edgeVectors.resize(num_arcs * 4);
}

void cleanupGraphData(struct GraphData* graphData) {
    graphData->edgeVectors.clear();
}

void printEdgeVectors(const struct GraphData* graphData) {
    int num_arcs = graphData->numOfArcs;

    printf("Edge Vectors:\n");
    for (int i = 0; i < num_arcs; i++) {
        int ori = graphData->edgeVectors[i * 4];
        int dest = graphData->edgeVectors[i * 4 + 1];
        int dist = graphData->edgeVectors[i * 4 + 2];
        int t = graphData->edgeVectors[i * 4 + 3];
        printf("%d %d %d %d\n", ori, dest, dist, t);
    }
}

void assign_global_variables(const struct GraphData* graphData) {
    int i, ori, dest, dist, t;
    num_gnodes = graphData->numOfGnode;
    int num_arcs = graphData->numOfArcs;

    for (i = 0; i < num_gnodes; i++)
        adjacent_table[i][0] = 0;

    for (i = 0; i < num_arcs; i++) {
        ori = graphData->edgeVectors[i * 4];
        dest = graphData->edgeVectors[i * 4 + 1];
        dist = graphData->edgeVectors[i * 4 + 2];
        t = graphData->edgeVectors[i * 4 + 3];

        adjacent_table[ori - 1][0]++;
        adjacent_table[ori - 1][adjacent_table[ori - 1][0] * 3 - 2] = dest - 1;
        adjacent_table[ori - 1][adjacent_table[ori - 1][0] * 3 - 1] = dist;
        adjacent_table[ori - 1][adjacent_table[ori - 1][0] * 3] = t;
    }
}

unsigned (*paretoPathsInFragment(int s_node, int d_node, const struct GraphData* graphData))[2] {
    printf("Number of Nodes: %d\n", graphData->numOfGnode);
    start = s_node - 1;
    goal = d_node - 1;
    num_gnodes = graphData->numOfGnode;
    assign_global_variables(graphData);
    new_graph();
    unsigned (*solutions)[2] = call_boastar();
    return solutions;
}

unsigned (*paretoPathsInFragmentChar(int s_node, int d_node, const char* filename))[2] {
    start = s_node - 1;
    goal = d_node - 1;
    read_adjacent_table(filename);
    if (graph_node == NULL) {
        printf("New graph generated: %d\n", 1);
        new_graph();
    }
    unsigned (*solutions)[2] = call_boastar();
    if (graph_node == NULL) {
        printf("Successfully cleared graph: %d\n", 1);
    }
    return solutions;
}
