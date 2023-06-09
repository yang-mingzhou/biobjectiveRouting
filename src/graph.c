#include "include.h"
#include "boastar.h"
#include "graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>


void read_adjacent_table(const char* filename) {
	FILE* f;
	int i, ori, dest, dist, t;
	f = fopen(filename, "r");
	int num_arcs = 0;
	if (f == NULL) 	{
		printf("Cannot open file %s.\n", filename);
		exit(1);
	}
	fscanf(f, "%d %d", &num_gnodes, &num_arcs);
	fscanf(f, "\n");

	for (i = 0; i < num_gnodes; i++)
		adjacent_table[i][0] = 0;

	for (i = 0; i < num_arcs; i++) {
		fscanf(f, "%d %d %d %d\n", &ori, &dest, &dist, &t);
		adjacent_table[ori - 1][0]++;
		adjacent_table[ori - 1][adjacent_table[ori - 1][0] * 3 - 2] = dest - 1;
		adjacent_table[ori - 1][adjacent_table[ori - 1][0] * 3 - 1] = dist;
		adjacent_table[ori - 1][adjacent_table[ori - 1][0] * 3] = t;

		pred_adjacent_table[dest - 1][0]++;
		pred_adjacent_table[dest - 1][pred_adjacent_table[dest - 1][0] * 3 - 2] = ori - 1;
		pred_adjacent_table[dest - 1][pred_adjacent_table[dest - 1][0] * 3 - 1] = dist;
		pred_adjacent_table[dest - 1][pred_adjacent_table[dest - 1][0] * 3] = t;
	}
	fclose(f);
}

// void new_graph() {
// 	int y;
// 	if (graph_node == NULL) {
//         printf("num_gnodes: %d\n", num_gnodes);
// 		graph_node = (gnode*) calloc(num_gnodes, sizeof(gnode));
// 		for (y = 0; y < num_gnodes; ++y) 		{
// 			graph_node[y].id = y;
// 			graph_node[y].gmin = LARGE;
// 			graph_node[y].h1 = LARGE;
// 			graph_node[y].h2 = LARGE;
// 			graph_node[y].gopfirst = NULL;
// 		}
// 	}
// }

void new_graph() {
    int y;
//     printf("num_gnodes: %d\n", num_gnodes);
    if (graph_node == NULL) {
        graph_node = (gnode*) calloc(num_gnodes, sizeof(gnode));
    } else {
        gnode* temp = (gnode*) realloc(graph_node, num_gnodes * sizeof(gnode));
        if (temp == NULL) {
            // handle error, e.g., free resources, print error message, exit, etc.
            printf("Failed to allocate memory!\n");
            free(graph_node);
            exit(1);
        }
        graph_node = temp;
    }
    for (y = 0; y < num_gnodes; ++y) {
        graph_node[y].id = y;
        graph_node[y].gmin = LARGE;
        graph_node[y].h1 = LARGE;
        graph_node[y].h2 = LARGE;
        graph_node[y].gopfirst = NULL;
    }
}

