#include <iostream>
#include <vector> 
#include <fstream>
#include <list>
#include <array>
#include <unordered_map>
#include <time.h>
#include <algorithm>
#include "pathRetrieval.h"
#include "hbor.h"

using namespace std;

// module load gcc/11.3.0
// gcc -std=c99 -o main.o hbor.cpp pathRetrieval.c heap.c boastar.c graph.c -lstdc++

int fragmentOf(int nodeID){
    return 0;
}

vector<int> boundaryNodesOf(int fragmentID){
    vector<int> boundaryNodes;
    return boundaryNodes;
}

vector<vector<int>> onePairBoundaryPathOf(int snode, int dnode, int sBN, int dBN){
    // for each boundaryPath, the first four elements are (LB1, LB2, UB1, UB2), respectively
    vector<vector<int>> onePairBoundaryPath;
    return onePairBoundaryPath;
}

vector<vector<int>> boundaryPathDominanceCheck(vector<vector<int>> boundaryPathSet){
    // for each boundaryPath, the first four elements are (LB1, LB2, UB1, UB2), respectively
    vector<vector<int>> paretoBoundaryPath;
    return paretoBoundaryPath;
}

vector<vector<int>> paretoBoundaryPathsBetween(int snode, int dnode){
    vector<vector<int>> boundaryPathSet;
    vector<vector<int>> paretoBoundaryPathSet;
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
            boundaryPathSet.insert(boundaryPathSet.end(), onePairBoundaryPathSet.begin(), onePairBoundaryPathSet.end());
        }
    }
    paretoBoundaryPathSet = boundaryPathDominanceCheck(boundaryPathSet);
    return paretoBoundaryPathSet;
}

vector<array<int, 2>> expandPathCostOf(vector<int> boundaryPath){
    vector<array<int, 2>> pathCostSet;
    return pathCostSet;
}

vector<array<int, 2>> expandPathForBoundaryPathSet(vector<vector<int>> boundaryPathSet){
    vector<array<int, 2>> expendedPathCostSet;
    for (size_t i = 0; i < boundaryPathSet.size(); ++i) {
        vector<array<int, 2>> pathCostWithOneBoundaryPath = expandPathCostOf(boundaryPathSet[i]);
        expendedPathCostSet.insert(expendedPathCostSet.end(), pathCostWithOneBoundaryPath.begin(), pathCostWithOneBoundaryPath.end());
    }
    return expendedPathCostSet;
}   

vector<array<int, 2>> dominanceCheck(vector<array<int, 2>> superParetoCostSet){
    vector<array<int, 2>> paretoPathCostSet;
    return paretoPathCostSet;
}


int hbor(int snode, int dnode, const char* filename){
    vector<vector<int>> boundaryPathSet = paretoBoundaryPathsBetween(snode, dnode);
    vector<array<int, 2>> superParetoCostSet = expandPathForBoundaryPathSet(boundaryPathSet);
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

// int main(){
//     testHbor();
//     return 0;
// }

