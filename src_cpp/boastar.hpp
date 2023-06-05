#ifndef BOASTAR_HPP
#define BOASTAR_HPP

#include <array>
#include <vector>
#include "heap.hpp"
#include "node.hpp"

#define MAX_SOLUTIONS 1000000
#define MAX_RECYCLE   1000000

extern std::vector<gnode> graph_node;
extern unsigned num_gnodes;
extern std::array<std::array<unsigned, MAXNEIGH>, MAXNODES> adjacent_table;
extern std::array<std::array<unsigned, MAXNEIGH>, MAXNODES> pred_adjacent_table;
extern unsigned goal, start; 

std::array<std::array<unsigned, 2>, MAX_SOLUTIONS> call_boastar();

#endif
