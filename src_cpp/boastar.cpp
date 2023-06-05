#include "heap.hpp"
#include "node.hpp"
#include "boastar.hpp"
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <unistd.h>
#include <vector>

extern Heap heap, heap_dij;
extern std::vector<gnode> graph_node;
extern unsigned num_gnodes;
extern std::array<std::array<unsigned, MAXNEIGH>, MAXNODES> adjacent_table;
extern std::array<std::array<unsigned, MAXNEIGH>, MAXNODES> pred_adjacent_table;
extern unsigned goal, start;
gnode* start_state;
gnode* goal_state;
snode* start_node;

unsigned long long int stat_expansions = 0;
unsigned long long int stat_generated = 0;
unsigned long long int minf_solution = LARGE;
unsigned long long int stat_created = 0;

std::array<std::array<unsigned, 2>, MAX_SOLUTIONS> solutions;
unsigned nsolutions = 0;
unsigned stat_pruned = 0;

// Define recycled_nodes as a vector
std::vector<snode> recycled_nodes(MAX_RECYCLE);

void initialize_parameters() {
    start_state = &graph_node[start];
    goal_state = &graph_node[goal];
    stat_percolations = 0;
    for (int i = 0; i < num_gnodes; ++i) {
        graph_node[i].gmin = LARGE;
    }

    for (int i = 0; i < nsolutions; ++i) {
        for (int j = 0; j < 2; ++j) {
            solutions[i][j] = 0;
        }
    }
    minf_solution = LARGE;
}

int backward_dijkstra(int dim) {
    for (int i = 0; i < num_gnodes; ++i)
        graph_node[i].key = LARGE;
    emptyheap_dij();
    goal_state->key = 0;
    insertheap_dij(goal_state);

    while (topheap_dij() != NULL) {
        gnode* n;
        gnode* pred;
        short d;
        n = popheap_dij();
        if (dim == 1)
            n->h1 = n->key;
        else
            n->h2 = n->key;
        ++stat_expansions;
        for (d = 1; d < pred_adjacent_table[n->id][0] * 3; d += 3) {
            pred = &graph_node[pred_adjacent_table[n->id][d]];
            int new_weight = n->key + pred_adjacent_table[n->id][d + dim];
            if (pred->key > new_weight) {
                pred->key = new_weight;
                insertheap_dij(pred);
            }
        }
    }
    return 1;
}

snode new_node() {
    static int recycled_index = MAX_RECYCLE - 1;
    if (recycled_index >= 0)
        return recycled_nodes[recycled_index--];
    return {};
}

void recycle_node(snode& node) {
    static int recycled_index = 0;
    if (recycled_index < MAX_RECYCLE)
        recycled_nodes[recycled_index++] = node;
}

int boastar() {
    int next_recycled = 0;
    nsolutions = 0;
    stat_pruned = 0;
    stat_generated = 0;
    stat_created = 0;

    emptyheap();

    start_node = new_node();
    ++stat_created;
    start_node->state = start;
    start_node->g1 = 0;
    start_node->g2 = 0;
    start_node->key = 0;
    start_node->searchtree = nullptr;
    insertheap(start_node);

    stat_expansions = 0;

    while (topheap() != NULL) {
        snode* n = popheap();

        short d;

        if (n->g2 >= graph_node[n->state].gmin || n->g2 + graph_node[n->state].h2 >= minf_solution) {
            stat_pruned++;
            if (next_recycled < MAX_RECYCLE) {
                recycle_node(*n);
                next_recycled++;
            }
            continue;
        }

        graph_node[n->state].gmin = n->g2;

        if (n->state == goal) {
            solutions[nsolutions][0] = n->g1;
            solutions[nsolutions][1] = n->g2;
            nsolutions++;
            if (nsolutions > MAX_SOLUTIONS) {
                printf("Maximum number of solutions reached, increase MAX_SOLUTIONS!\n");
                exit(1);
            }

            if (minf_solution > n->g2)
                minf_solution = n->g2;
            continue;
        }

        ++stat_expansions;

        for (d = 1; d < adjacent_table[n->state][0] * 3; d += 3) {
            snode succ;
            double newk1, newk2, newkey;
            unsigned nsucc = adjacent_table[n->state][d];
            unsigned cost1 = adjacent_table[n->state][d + 1];
            unsigned cost2 = adjacent_table[n->state][d + 2];

            unsigned newg1 = n->g1 + cost1;
            unsigned newg2 = n->g2 + cost2;
            unsigned h1 = graph_node[nsucc].h1;
            unsigned h2 = graph_node[nsucc].h2;

            if (newg2 >= graph_node[nsucc].gmin || newg2 + h2 >= minf_solution)
                continue;

            newk1 = newg1 + h1;
            newk2 = newg2 + h2;

            succ.state = nsucc;
            stat_generated++;

            newkey = newk1 * static_cast<double>(BASE) + newk2;
            succ.searchtree = n;
            succ.g1 = newg1;
            succ.g2 = newg2;
            succ.key = newkey;
            insertheap(succ);
        }
    }

    return nsolutions > 0;
}

unsigned (*call_boastar())[2] {
    float runtime;
    struct timeval tstart, tend;
    unsigned long long min_cost;
    unsigned long long min_time;

    initialize_parameters();

    gettimeofday(&tstart, nullptr);

    // Dijkstra h1
    if (backward_dijkstra(1))
        min_cost = start_state->h1;

    // Dijkstra h2
    if (backward_dijkstra(2))
        min_time = start_state->h2;

    // BOA*
    boastar();

    gettimeofday(&tend, nullptr);
    runtime = 1.0 * (tend.tv_sec - tstart.tv_sec) + 1.0 * (tend.tv_usec - tstart.tv_usec) / 1000000.0;

    printf("%lld;%lld;%d;%f;%llu;%llu;%llu\n",
        start_state->id + 1,
        goal_state->id + 1,
        nsolutions,
        runtime * 1000,
        stat_generated,
        stat_expansions,
        stat_created);
    return solutions;
}
