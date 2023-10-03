#include "bodPathRetrieval.h"
#include "include.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

void bod_initializeGraphData(GraphData* graphData, int num_nodes, int num_arcs) {
    graphData->numOfGnode = num_nodes;
    graphData->numOfArcs = num_arcs;

    graphData->edgeVectors = (int**)malloc(num_arcs * sizeof(int*));
    for (int i = 0; i < num_arcs; i++) {
        graphData->edgeVectors[i] = (int*)malloc(4 * sizeof(int));
    }
}

void bod_cleanupGraphData(GraphData* graphData) {
    for (int i = 0; i < graphData->numOfArcs; i++) {
        free(graphData->edgeVectors[i]);
    }
    free(graphData->edgeVectors);
}

void bod_printEdgeVectors(const GraphData* graphData) {
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



void bod_assign_global_variables(const GraphData* graphData) {
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
//         printf("ori: %d%d%d%d\n", ori,dest,dist,t);
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

void bod_freeSolutions(BodSolutions* solutions) {
    SolutionNode* current = solutions->head;
    SolutionNode* next_node;
    
    while (current != NULL) {
        next_node = current->next;  
        free(current);             
        current = next_node;      
    }

    free(solutions); 
}



BodSolutions* bod_paretoPathsInFragment(int s_node,  const GraphData* graphData){
//     printf("Number of Nodes: %d\n", graphData->numOfGnode);
//     printf("Number of Arcs: %d\n", graphData->numOfArcs);
	start = s_node - 1;
    num_gnodes = graphData->numOfGnode;
//     const GraphData* graphDataPtr = &graphData;
// 	read_adjacent_table(filename);
	bod_assign_global_variables(graphData);
//     printf("Finished start: %d\n", start);
//     printf("Finished end: %d\n", goal);
//     bod_printEdgeVectors(graphData);
    new_graph();
	BodSolutions* result = call_bod();
    freeMemoryForTable(num_gnodes);
	return result;
}


BodSolutions* bod_paretoPathsInFragmentChar(int s_node, const char* filename){
    start = s_node - 1;
    read_adjacent_table(filename);
	new_graph();
	BodSolutions* result = call_bod();
	return result;
}


AllToAllSolutions* compute_all_to_all_paretoPaths_optimized(const GraphData* graphData) {
    int num_nodes = graphData->numOfGnode;
    AllToAllSolutions* all_solutions = (AllToAllSolutions*) malloc(sizeof(AllToAllSolutions));
    all_solutions->num_nodes = num_nodes;

    // Allocate memory for the 2D array of solutions
    all_solutions->solutions = (BodSolutions**) malloc(num_nodes * sizeof(BodSolutions*));
    for (int i = 0; i < num_nodes; ++i) {
        all_solutions->solutions[i] = (BodSolutions*) malloc(num_nodes * sizeof(BodSolutions));
    }

    // Initialize global variables and allocate memory once
    num_gnodes = graphData->numOfGnode;
    bod_assign_global_variables(graphData);
    new_graph();

    // Compute Pareto paths from each node to all other nodes
    for (int i = 0; i < num_nodes; ++i) {
        start = i;  // Set the global start variable directly
        BodSolutions* result = call_bod();
        all_solutions->solutions[i] = result;
        // Log the current status every 100 iterations
        if ((i + 1) % 100 == 0) {
            printf("Processed node %d out of %d\n", i + 1, num_nodes);
        }
    }

    // Free allocated memory for the graph data structure
    freeMemoryForTable(num_gnodes);
    return all_solutions;
}

void free_all_to_all_solutions(AllToAllSolutions* all_solutions) {
    if (all_solutions == NULL) {
        return;
    }

    int num_nodes = all_solutions->num_nodes;
    if (all_solutions->solutions != NULL) {
        for (int i = 0; i < num_nodes; ++i) {
            BodSolutions* solutions_array = all_solutions->solutions[i];
            if (solutions_array != NULL) {
                bod_freeSolutions(solutions_array);
            }
        }
        free(all_solutions->solutions);
    }
    free(all_solutions);
}

