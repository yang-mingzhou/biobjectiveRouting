/////////////////////////////////////////////////////////////////////
// Carlos Hernandez
// All rights reserved
/////////////////////////////////////////////////////////////////////
#include "pathRetrieval.h"
#include "include.h"
#include "boastar.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

void initializeGraphData(struct GraphData* graphData, int num_nodes, int num_arcs) {
    graphData->numOfGnode = num_nodes;
    graphData->numOfArcs = num_arcs;

    graphData->edgeVectors = (int**)malloc(num_arcs * sizeof(int*));
    for (int i = 0; i < num_arcs; i++) {
        graphData->edgeVectors[i] = (int*)malloc(4 * sizeof(int));
    }
}

void cleanupGraphData(struct GraphData* graphData) {
    for (int i = 0; i < graphData->numOfArcs; i++) {
        free(graphData->edgeVectors[i]);
    }
    free(graphData->edgeVectors);
}

void printEdgeVectors(const struct GraphData* graphData) {
    int num_arcs = graphData->numOfArcs;

    printf("Edge Vectors:\n");
    for (int i = 0; i < num_arcs; i++) {
        int ori = graphData->edgeVectors[i][0];
        int dest = graphData->edgeVectors[i][1];
        int dist = graphData->edgeVectors[i][2];
        int t = graphData->edgeVectors[i][3];
        printf("%d %d %d %d\n", ori, dest, dist, t);
    }
}



void assign_global_variables(const struct GraphData* graphData) {
//     printf("Number of Nodes inner: %d\n", graphData->numOfGnode);
//     printf("Number of Arcs inner: %d\n", graphData->numOfArcs);
//     printEdgeVectors(graphData);
	int i, ori, dest, dist, t;
    num_gnodes = graphData->numOfGnode;
	int num_arcs = graphData->numOfArcs;

	for (i = 0; i < num_gnodes; i++)
		adjacent_table[i][0] = 0;

    
	for (i = 0; i < num_arcs; i++) {
//         printf("Number of arc: %d\n", i);
        ori = graphData->edgeVectors[i][0];
        dest = graphData->edgeVectors[i][1];
        dist = graphData->edgeVectors[i][2];
        t = graphData->edgeVectors[i][3];
//         printf("ori: %d\n", ori);
// 		fscanf(f, "%d %d %d %d\n", &ori, &dest, &dist, &t);
		adjacent_table[ori - 1][0]++;
		adjacent_table[ori - 1][adjacent_table[ori - 1][0] * 3 - 2] = dest - 1;
		adjacent_table[ori - 1][adjacent_table[ori - 1][0] * 3 - 1] = dist;
		adjacent_table[ori - 1][adjacent_table[ori - 1][0] * 3] = t;

		pred_adjacent_table[dest - 1][0]++;
		pred_adjacent_table[dest - 1][pred_adjacent_table[dest - 1][0] * 3 - 2] = ori - 1;
		pred_adjacent_table[dest - 1][pred_adjacent_table[dest - 1][0] * 3 - 1] = dist;
		pred_adjacent_table[dest - 1][pred_adjacent_table[dest - 1][0] * 3] = t;
	}
//     printf("finished assignedc: %d\n", i);
}

unsigned (*paretoPathsInFragment(int s_node, int d_node, const struct GraphData* graphData))[2]{
//     printf("Number of Nodes: %d\n", graphData->numOfGnode);
//     printf("Number of Arcs: %d\n", graphData->numOfArcs);
	start = s_node - 1;
	goal = d_node - 1;
    num_gnodes = graphData->numOfGnode;
//     const GraphData* graphDataPtr = &graphData;
// 	read_adjacent_table(filename);
	assign_global_variables(graphData);
//     printf("Finished start: %d\n", start);
//     printf("Finished end: %d\n", goal);
//     printEdgeVectors(graphData);
    new_graph();
	unsigned (*solutions)[2] = call_boastar();
	return solutions;
}


unsigned (*paretoPathsInFragmentChar(int s_node, int d_node, const char* filename))[2]{
	start = s_node - 1;
	goal = d_node - 1;
	read_adjacent_table(filename);
	new_graph();
	unsigned (*solutions)[2] = call_boastar();
	return solutions;
}



