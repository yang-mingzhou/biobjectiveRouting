#include "bodPathRetrieval.h"
#include "include.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>

// void bod_initializeGraphData(GraphData* graphData, int num_nodes, int num_arcs) {
//     graphData->numOfGnode = num_nodes;
//     graphData->numOfArcs = num_arcs;

//     graphData->edgeVectors = (int**)malloc(num_arcs * sizeof(int*));
//     for (int i = 0; i < num_arcs; i++) {
//         graphData->edgeVectors[i] = (int*)malloc(4 * sizeof(int));
//     }
// }

void bod_initializeGraphData(GraphData* graphData, int num_nodes, int num_arcs) {
//     printf("bod_initializeGraphData:\n");
    graphData->numOfGnode = num_nodes;
    graphData->numOfArcs = num_arcs;
    
//     print_memory_usage("start of bod_initializeGraphData");

    // Allocate all edge vectors at once
    int* block = (int*)malloc(num_arcs * 4 * sizeof(int));
    if (!block) {
        // Handle memory allocation failure, perhaps by returning an error code.
        fprintf(stderr, "Error allocating memory for edge vectors.\n");
        return; // Or exit, or handle the error as appropriate for your application.
    }
//     print_memory_usage("allocate block");

    graphData->edgeVectors = (int**)malloc(num_arcs * sizeof(int*));
    if (!graphData->edgeVectors) {
        // Handle memory allocation failure.
        fprintf(stderr, "Error allocating memory for edge vector pointers.\n");
        free(block); // Clean up previously allocated memory before returning.
        return; // Or exit, or handle the error as appropriate for your application.
    }
//     print_memory_usage("allocate edgeVectors");
    

    // Assign pointers to the appropriate locations in the single memory block
    for (int i = 0; i < num_arcs; i++) {
        graphData->edgeVectors[i] = block + i * 4;
    }
}


// void bod_cleanupGraphData(GraphData* graphData) {
//     for (int i = 0; i < graphData->numOfArcs; i++) {
//         free(graphData->edgeVectors[i]);
//     }
//     free(graphData->edgeVectors);
// }

void bod_cleanupGraphData(GraphData* graphData) {
    // Only free the block of memory if it was allocated.
    if (graphData->edgeVectors) {
        // Since all edge vectors were allocated in a single block, only one free is required.
        // Free the block that graphData->edgeVectors[0] points to, which is the first vector.
        free(graphData->edgeVectors[0]);
    }

    // Now free the array of pointers.
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
    
//     allocateMemoryForTable(num_gnodes, num_arcs);
    
    // Step 1: Create temporary arrays to hold the counts
    unsigned *adj_count = calloc(num_gnodes, sizeof(unsigned));
    unsigned *pred_count = calloc(num_gnodes, sizeof(unsigned));
    


    // Step 2: Scan the arc data
    for (int i = 0; i < num_arcs; ++i) {
        int ori = graphData->edgeVectors[i][0];
        int dest = graphData->edgeVectors[i][1];
        adj_count[ori - 1]++;
        pred_count[dest - 1]++;
    }

    // Step 3: Allocate memory based on the counts
    adjacent_table = malloc(num_gnodes * sizeof(unsigned *));
    pred_adjacent_table = malloc(num_gnodes * sizeof(unsigned *));
    
    if (adjacent_table == NULL || pred_adjacent_table == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    for (unsigned i = 0; i < num_gnodes; ++i) {
        // For each node, allocate 3 times the count of neighbors (for storing dest, dist, and t)
        adjacent_table[i] = malloc((1+adj_count[i] * 3) * sizeof(unsigned));
        pred_adjacent_table[i] = malloc((1+pred_count[i] * 3) * sizeof(unsigned));
        
        if (adjacent_table[i] == NULL || pred_adjacent_table[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }


        // Initialize the count of neighbors at the first index
        adjacent_table[i][0] = 0;
        pred_adjacent_table[i][0] = 0;
    }


// 	for (i = 0; i < num_gnodes; i++){
//         adjacent_table[i][0] = 0;
//         pred_adjacent_table[i][0] = 0;
//     }
		

    
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
        
    free(adj_count);
    free(pred_count);

}

void bod_freeSolutions(BodSolutions* solutions) {
    SolutionNode* current = solutions->head;
    SolutionNode* next_node;
//     printf("Head of solutions list before freeing: %p\n", (void*)solutions->head);
    while (current != NULL) {
        next_node = current->next;  
//         printf("Freed SolutionNode at %p\n", (void*)current);
        free(current);             
        current = next_node;      
    }
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
//     print_memory_usage("Start of function");
    int num_nodes = graphData->numOfGnode;
    AllToAllSolutions* all_solutions = (AllToAllSolutions*) malloc(sizeof(AllToAllSolutions));
    all_solutions->num_nodes = num_nodes;
    all_solutions->solutions = (BodSolutions**) malloc(num_nodes * sizeof(BodSolutions*));
//     for (int i = 0; i < num_nodes; ++i) {
//         all_solutions->solutions[i] = (BodSolutions*) malloc(num_nodes * sizeof(BodSolutions));
//     }
//     BodSolutions* block = malloc(num_nodes * num_nodes * sizeof(BodSolutions));
//     for(int i = 0; i < num_nodes; ++i) {
//         all_solutions->solutions[i] = &block[i * num_nodes];
//     }
//     print_memory_usage("After allocating all_solutions");

    // Initialize global variables and allocate memory once
    num_gnodes = graphData->numOfGnode;
    bod_assign_global_variables(graphData);
    new_graph();
//     print_memory_usage("Before call_bod()");
    // Compute Pareto paths from each node to all other nodes
    for (int i = 0; i < num_nodes; ++i) {      
        start = i;  // Set the global start variable directly
        BodSolutions* result = call_bod();
        all_solutions->solutions[i] = result;
        // Log the current status every 100 iterations
        if ((i) % 100 == 0) {
//             printf("Processed node %d out of %d\n", i + 1, num_nodes);
            char buffer[100];
            snprintf(buffer, sizeof(buffer), "After call_bod() for i=%d", i);
//             print_memory_usage(buffer);
        }
    }

    // Free allocated memory for the graph data structure
    freeMemoryForTable(num_gnodes);
//     print_memory_usage("End of function");
    return all_solutions;
}

// void free_all_to_all_solutions(AllToAllSolutions* all_solutions) {
//     if (all_solutions == NULL) {
//         printf("all_solutions is NULL. Skipping free.\n");
//         return;
//     }

//     int num_nodes = all_solutions->num_nodes;
//     if (all_solutions->solutions != NULL) {
//         for (int i = 0; i < num_nodes; ++i) {
//             BodSolutions* solutions_array = all_solutions->solutions[i];
//             if (solutions_array != NULL) {
//                 bod_freeSolutions(solutions_array);
//             }
//         }
//         free(all_solutions->solutions);
//     }
//     free(all_solutions);
// }

void free_all_to_all_solutions(AllToAllSolutions* all_solutions) {
    if (all_solutions == NULL) {
//         printf("all_solutions is NULL. Skipping free.\n");
        return;
    }

    int num_nodes = all_solutions->num_nodes;

    // Free each array of BodSolutions
    if (all_solutions->solutions != NULL) {
        for (int i = 0; i < num_nodes; ++i) {
            BodSolutions* solutions_array = all_solutions->solutions[i];
            if (solutions_array != NULL) {
                for(int j = 0; j < num_nodes; j++) {
                    bod_freeSolutions(&solutions_array[j]);
                }
                
                // Free the array itself
                free(solutions_array);
                all_solutions->solutions[i] = NULL;
            }
        }

        // Free the array of BodSolutions arrays
        free(all_solutions->solutions);
        all_solutions->solutions = NULL;
    }

    // Free the AllToAllSolutions struct
    free(all_solutions);
    all_solutions = NULL;
}




// void free_all_to_all_solutions(AllToAllSolutions* all_solutions) {
//     if (all_solutions == NULL) {
//         return;
//     }

//     int num_nodes = all_solutions->num_nodes;

//     if (all_solutions->solutions != NULL) {
//         // Free each linked list within each BodSolutions object
//         for (int i = 0; i < num_nodes; ++i) {
//             BodSolutions* solutions_array = all_solutions->solutions[i];
//             if (solutions_array != NULL) {
//                 bod_freeSolutions(solutions_array);
//             }
//         }

//         // Free the single block of BodSolutions, assuming the first row is the start of the block.
//         free(all_solutions->solutions[0]);

//         // Free the array of pointers
//         free(all_solutions->solutions);
//     }

//     // Free the AllToAllSolutions struct itself
//     free(all_solutions);
// }

void print_memory_usage(const char* annotation) {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) != 0) {
        fprintf(stderr, "Could not get memory usage.\n");
        return;
    }
    printf("Memory usage at %s: %ld KB\n", annotation, usage.ru_maxrss);
}

