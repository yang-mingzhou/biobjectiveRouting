#include "include.hpp"
#include "boastar.hpp"
#include "graph.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <vector>






// graph.cpp

std::vector<gnode> graph_node;
std::vector<std::vector<unsigned>> adjacent_table;
std::vector<std::vector<unsigned>> pred_adjacent_table;

void read_adjacent_table(const char* filename) {
    FILE* f;
    int i, ori, dest, dist, t;
    f = fopen(filename, "r");
    int num_arcs = 0;
    if (f == NULL) {
        printf("Cannot open file %s.\n", filename);
        exit(1);
    }
    fscanf(f, "%d %d", &num_gnodes, &num_arcs);
    fscanf(f, "\n");

    adjacent_table.resize(num_gnodes);
    pred_adjacent_table.resize(num_gnodes);

    for (i = 0; i < num_gnodes; i++)
        adjacent_table[i].push_back(0);

    for (i = 0; i < num_arcs; i++) {
        fscanf(f, "%d %d %d %d\n", &ori, &dest, &dist, &t);
        adjacent_table[ori - 1][0]++;
        adjacent_table[ori - 1].push_back(dest - 1);
        adjacent_table[ori - 1].push_back(dist);
        adjacent_table[ori - 1].push_back(t);

        pred_adjacent_table[dest - 1][0]++;
        pred_adjacent_table[dest - 1].push_back(ori - 1);
        pred_adjacent_table[dest - 1].push_back(dist);
        pred_adjacent_table[dest - 1].push_back(t);
    }
    fclose(f);
}

void new_graph() {
    if (graph_node.empty()) {
        graph_node.resize(num_gnodes);
        for (int y = 0; y < num_gnodes; ++y) {
            graph_node[y].id = y;
            graph_node[y].gmin = LARGE;
            graph_node[y].h1 = LARGE;
            graph_node[y].h2 = LARGE;
            graph_node[y].gopfirst = NULL;
        }
    }
}


