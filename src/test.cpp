#include <cassert>
#include <vector>
#include <list>
#include <array>
#include <iostream>  

#include "hbor.h"

using namespace std;

// module load gcc/11.3.0
// gcc -std=c99 -o test.o test.cpp hbor.cpp pathRetrieval.c heap.c boastar.c graph.c -lstdc++

void testfragmentof() {
    int nodeID = 1;
    int expected = 0;
    int result = fragmentOf(nodeID);
    assert(result == expected);  // Assertion using assert macro
    cout << "Test passed!" << endl;
}

int main() {
    // Run the test functions
    testfragmentof();
    return 0;
}