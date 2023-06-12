#include "namoadrPathRetrieval.h"
#include "include.h"
#include "namoadr.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>



void initializeGraphDataBOA(GraphData* graphData, int num_nodes, int num_arcs) {
    graphData->numOfGnode = num_nodes;
    graphData->numOfArcs = num_arcs;

    graphData->edgeVectors = (int**)malloc(num_arcs * sizeof(int*));
    for (int i = 0; i < num_arcs; i++) {
        graphData->edgeVectors[i] = (int*)malloc(4 * sizeof(int));
    }
}

void cleanupGraphData(GraphData* graphData) {
    for (int i = 0; i < graphData->numOfArcs; i++) {
        free(graphData->edgeVectors[i]);
    }
    free(graphData->edgeVectors);
}

void printEdgeVectors(const GraphData* graphData) {
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



void assign_global_variables(const GraphData* graphData) {
//     printf("Number of Nodes inner: %d\n", graphData->numOfGnode);
//     printf("Number of Arcs inner: %d\n", graphData->numOfArcs);
//     printEdgeVectors(graphData);
	int i, ori, dest, dist, t;
    num_gnodes = graphData->numOfGnode;
	int num_arcs = graphData->numOfArcs;
    
    allocateMemoryForTable(num_gnodes, num_arcs);

	for (i = 0; i < num_gnodes; i++){
        adjacent_table[i][0] = 0;
        pred_adjacent_table[i][0] = 0;
    }

    
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

// unsigned (*paretoPathsInFragment(int s_node, int d_node, const GraphData* graphData))[2]{
// //     
// //     printf("Number of Nodes: %d\n", graphData->numOfGnode);
// //     printf("Number of Arcs: %d\n", graphData->numOfArcs);
// 	start = s_node - 1;
// 	goal = d_node - 1;
//     num_gnodes = graphData->numOfGnode;
// //     const GraphData* graphDataPtr = &graphData;
// // 	read_adjacent_table(filename);
// 	assign_global_variables(graphData);
// //     printf("Finished start: %d\n", start);
// //     printf("Finished end: %d\n", goal);
// //     printEdgeVectors(graphData);
//     new_graph();
// 	unsigned (*solutions)[2] = call_boastar();
//     freeMemoryForTable(num_gnodes);
// 	return solutions;
// }

unsigned (*namorInFragment(int s_node, int d_node, const GraphData* graphData))[2]{
//     
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
	unsigned (*solutions)[2] = call_namoadr();
    freeMemoryForTable(num_gnodes);
	return solutions;
}


// unsigned (*paretoPathsInFragmentChar(int s_node, int d_node, const char* filename))[2] {
//     start = s_node - 1;
//     goal = d_node - 1;
//     read_adjacent_table(filename);
//     if (graph_node == NULL) {
//         printf("New graph generated: %d\n", 1);
//         new_graph();
//     }
//     unsigned (*solutions)[2] = call_boastar();
//     if (graph_node == NULL) {
//         printf("Successfully cleared graph: %d\n", 1);
//     }
//     return solutions;
// }







