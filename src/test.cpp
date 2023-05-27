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
    // tested
    B3HEPV hepv = B3HEVP("../b3hepv");
    vector<array<int, 2>>  result = hepv.fragmentIndex;
    for (size_t i = 0; i < result.size(); ++i) {
        cout << "For node" << i+1 << ": Fragment ID "<< result[i][0] << ", Fragment node ID: "<< result[i][1] <<"\n";
        
    }
    cout<<endl;
    // assert(result == expected);  // Assertion using assert macro
    // cout << "Test passed!" << endl;
}

void test_readBoundaryNode(){
    // tested
    B3HEPV hepv = B3HEVP("../b3hepv");
    vector<vector<int>> matrix = hepv.boundaryNodeSet;
    cout<< "start";
    for (const auto& row : matrix) {
        for (int num : row) {
            cout << num << " ";
        }
        cout << endl;
    }

}

void test_boundaryPathDominanceCheck(){
    // tested
    array<int, 4> lub1 = {10, 20, 30, 40};
    vector<int> path1 = {5, 6, 7, 8};
    BoundaryPath bp1(lub1, path1);
    
    array<int, 4> lub2 = {20, 10, 40, 30};
    vector<int> path2 = {5, 14, 15, 8};
    BoundaryPath bp2(lub2, path2);

    array<int, 4> lub3 = {50, 60, 110, 120};
    vector<int> path3 = {5, 40, 15, 8};
    BoundaryPath bp3(lub3, path3);

    cout<< bp3.isDominatedBy(bp2)<< endl;

    vector<BoundaryPath> boundaryPathSet = {bp1, bp2, bp3};

    cout<< "before:" <<endl;

    for (const BoundaryPath& boundaryPath: boundaryPathSet) {
        boundaryPath.printPath();
    }

    vector<BoundaryPath> paretoBoundaryPath = B3HEPV::boundaryPathDominanceCheck(boundaryPathSet);
    cout<< "after:" <<endl;
    for (const BoundaryPath& boundaryPath: paretoBoundaryPath) {
        boundaryPath.printPath();
    }
    

}

void testHbor(){
    B3HEPV hepv = B3HEVP("../b3hepv");
    int snode = 1;
    int dnode = 8;
    int nsolutions = B3HEPV::boaPathRetrieval(snode, dnode);
    cout << "The number of solution between: " << snode << " and "<< dnode << " is "<< nsolutions <<endl;
}

int main() {
    // Run the test functions
    // test_readFragementIndex();
    // testHbor();
    test_boundaryPathDominanceCheck();
    return 0;
}