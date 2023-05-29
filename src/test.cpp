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
    B3HEPV hepv = B3HEPV("../b3hepv");
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
    B3HEPV hepv = B3HEPV("../b3hepv");
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
    vector<int> lub1 = {10, 20, 30, 40};
    vector<int> path1 = {5, 6, 7, 8};
    BoundaryPath bp1(lub1, path1);
    
    vector<int> lub2 = {20, 10, 40, 30};
    vector<int> path2 = {5, 14, 15, 8};
    BoundaryPath bp2(lub2, path2);

    vector<int> lub3 = {50, 60, 110, 120};
    vector<int> path3 = {5, 40, 15, 8};
    BoundaryPath bp3(lub3, path3);


    vector<BoundaryPath> boundaryPathSet = {bp1, bp2, bp3};

    cout<< "before:" <<endl;

    for (const BoundaryPath& boundaryPath: boundaryPathSet) {
        boundaryPath.printPath();
    }

    B3HEPV hepv = B3HEPV("../b3hepv");

    vector<BoundaryPath> paretoBoundaryPath = hepv.boundaryPathDominanceCheck(boundaryPathSet);
    cout<< "after:" <<endl;
    for (const BoundaryPath& boundaryPath: paretoBoundaryPath) {
        boundaryPath.printPath();
    }
}

void test_ReadBoundaryPathView(){
    // tested
    B3HEPV hepv = B3HEPV("../b3hepv");
    for (const auto& entry1 : hepv.boundaryEncodedPathView) {
        int key1 = entry1.first;
        for (const auto& entry2 : entry1.second) {
            int key2 = entry2.first;
            const vector<BoundaryPath>& paths = entry2.second;

            cout << "snode: " << key1 << ",dnode : " << key2 << endl;
            for (const auto& path : paths) {
                path.printPath();
            }
        }
    }
}

void test_concatBoundaryPath(){
    // tested
    BoundaryPath path1({1, 2, 3, 4}, {10, 20, 30, 40, 50});
    BoundaryPath path2({5, 6, 7, 8}, {50, 60, 70, 80});
    BoundaryPath path3({5, 6, 7, 8}, {80, 90, 100});

    try {
        BoundaryPath concatenatedPath = path1.concatWith(path2).concatWith(path3);
        cout << "Concatenated Lubs: ";
        for (int i : concatenatedPath.lub) {
            cout << i << " ";
        }
        cout << endl;
        concatenatedPath.printPath();
    } catch (const runtime_error& error) {
        cout << "Error: " << error.what() << endl;
    }
}

void test_readFragmentEncodedPathView(){
    // tested
    B3HEPV hepv = B3HEPV("../b3hepv");
    for (const auto& entry : hepv.fragmentEncodedPathView) {
        int key1 = entry.first;

        for (const auto& innerEntry : entry.second) {
            int key2 = innerEntry.first;
            const BoundaryPath& boundaryPath = innerEntry.second;

            boundaryPath.printPath();
        }
    }
}

void test_onePairBoundaryPathOf(){
    // tested
    B3HEPV hepv = B3HEPV("../b3hepv");
    vector<BoundaryPath> boundaryPathSet = hepv.onePairBoundaryPathOf(1, 3, 4, 5);
    for (const auto& path : boundaryPathSet) {
        for (int i : path.lub) {
            cout << i << " ";
        }
        cout << endl;
        path.printPath();
    }
}

void test_paretoBoundaryPathBetween(){
    B3HEPV hepv = B3HEPV("../b3hepv");
    vector<BoundaryPath> boundaryPathSet = hepv.paretoBoundaryPathBetween(1, 8);
    for (const auto& path : boundaryPathSet) {
        for (int i : path.lub) {
            cout << i << " ";
        }
        cout << endl;
        path.printPath();
    }
}

int main() {
    // Run the test functions

    // test_readFragementIndex();
    // test_readBoundaryNode();
    // test_boundaryPathDominanceCheck();
    // test_ReadBoundaryPathView();
    // test_concatBoundaryPath();
    // test_readFragmentEncodedPathView();
    //test_onePairBoundaryPathOf();

    test_paretoBoundaryPathBetween(); // remained untested
    // testHbor();
    
    return 0;
}