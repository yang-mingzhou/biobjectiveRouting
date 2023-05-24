#include "pathRetrieval.h"
#include <iostream>
// gcc -std=c99 -o pathRetrieval fragmentPathRetrieval.cpp pathRetrieval.c heap.c boastar.c graph.c -lstdc++

void testPathRetrieval() {
    unsigned (*solutions)[2] = paretoPathsInFragment(231980, 181404, "../Maps/COL-road-d.txt");
    // Access and print the values
    for (int i = 0; i < MAX_SOLUTIONS; i++) {
        if (solutions[i][0] >0)
            std::cout << "Row " << i << ": " << solutions[i][0] << ", " << solutions[i][1] << std::endl;
    }
}

int main(){
    testPathRetrieval();
    return 0;
}
