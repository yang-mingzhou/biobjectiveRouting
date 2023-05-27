/////////////////////////////////////////////////////////////////////
// Carlos Hernandez
// All rights reserved
/////////////////////////////////////////////////////////////////////

#include "include.h"
#include "boastar.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>


//compile gcc -Wall -std=c99 -o pathRetrieval.o pathRetrieval.cpp heap.c boastar.c graph.c

unsigned (*paretoPathsInFragment(int s_node, int d_node, const char* filename))[2]{
	start = s_node - 1;
	goal = d_node - 1;
	read_adjacent_table(filename);
	new_graph();
	unsigned (*solutions)[2] = call_boastar();
	return solutions;
}

