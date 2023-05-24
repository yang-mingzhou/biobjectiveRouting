#include <iostream>
#include <vector>
#include <fstream>
#include <list>
#include <unordered_map>
#include <time.h>
#include <algorithm>
#include "pathRetrieval.h"
using namespace std;

// module load gcc/11.3.0
// gcc -std=c99 -o main hbor.cpp pathRetrieval.c heap.c boastar.c graph.c -lstdc++

int fragmentof(int nodeID){
}

vector<int> boundaryNodesOf(int fragmentID){

}

vector<vector<int>> onePairBoundaryPathOf(int snode, int dnode, int sBN, int dBN){
    // for each boundaryPath, the first four elements are (LB1, LB2, UB1, UB2), respectively

}

vector<vector<int>> paretoBoundaryPathsBetween(int snode, int dnode){
    vector<vector<int>> boundaryPathSet;
    int sBN, dBN;
    int sfragment = fragmentOf(snode);
    int dfragment = fragmentOf(dnode);
    vector<int> sBoundaryNodes = boundaryNodesOf(sfragment);
    vector<int> dBoundaryNodes = boundaryNodesOf(dfragment);
    
    for (size_t i = 0; i < sBoundaryNodes.size(); ++i) {
        sBN = sBoundaryNodes[i];
        for (size_t j = 0; j < sBoundaryNodes.size(); ++j){
            dBN = dBoundaryNodes[j];
            vector<vector<int>> onePairBoundaryPathSet = onePairBoundaryPathOf(snode, dnode, sBN, dBN); 
            boundaryPathSet.insert(boundaryPathSet.end(), onePairBoundaryPath.begin(), onePairBoundaryPathSet.end());
        }
    }
    paretoBoundaryPathSet 
    return boundaryPathSet;
}


int hbor(int snode, int dnode, const char* filename){
    vector<vector<int>> boundaryPathSet = paretoBoundaryPathsBetween(snode, dnode);
    vector<array<int, 2>> superParetoCostSet = expandPathForBoundaryPaths(boundaryPaths);
    vector<array<int, 2>> solutions = dominanceCheck(superParetoCostSet); 
    int nsolutions = solutions.size();
    return nsolutions;
}

void testHbor(){
    const char* filename = "../Maps/COL-road-d.txt";
    int snode = 231980;
    int dnode = 181404;
    int nsolutions = hbor(snode, dnode, filename);
    cout << "The number of solution between: " << snode << " and "<< dnode << " is "<< nsolutions <<endl;
}

int main(){
    testHbor();
    return 0;
}

