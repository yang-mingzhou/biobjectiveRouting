#include <cassert>
#include <vector>
#include <list>
#include <array>
#include <iostream>  

#include "hbor.h"

using namespace std;

// module load gcc/11.3.0
// gcc -std=c99 -o test.o test.cpp hbor.cpp pathRetrieval.c heap.c boastar.c graph.c -lstdc++

void test_readFragementIndex() {
    vector<array<int, 2>>  result = readFragementIndex("../b3hepv/fragmentIndex.txt");
    for (size_t i = 0; i < result.size(); ++i) {
        cout << "For node" << i+1 << ": Fragment ID "<< result[i][0] << ", Fragment node ID: "<< result[i][1] <<"\n";
        
    }
    cout<<endl;
    // assert(result == expected);  // Assertion using assert macro
    // cout << "Test passed!" << endl;
}

void testHbor(){
    const char* filename = "../b3hepv/testImputGraph.txt";
    int snode = 1;
    int dnode = 8;
    int nsolutions = boaPathRetrieval(snode, dnode, filename);
    cout << "The number of solution between: " << snode << " and "<< dnode << " is "<< nsolutions <<endl;
}

void test_readBoundaryNode(){
    vector<vector<int>> matrix = readBoundaryNode("../b3hepv/boundaryNodes.txt");
    cout<< "start";
    for (const auto& row : matrix) {
        for (int num : row) {
            cout << num << " ";
        }
        cout << endl;
    }

}

int main() {
    // Run the test functions
    // test_readFragementIndex();
    // testHbor();
    test_readBoundaryNode();
    return 0;
}