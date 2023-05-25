#include <iostream>
#include <vector> 
#include <fstream>
#include <sstream>
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

class BoundaryPath {
    // for each boundaryPath, the lub: four elements are (LB1, LB2, UB1, UB2); path: a sequence of boundary nodes
    public:
    BoundaryPath(const array<int, 4>& bounds, const vector<int>& pathSequence)
        : lub(bounds), path(pathSequence) {}

    bool isDominatedBy(const BoundaryPath& other) const {
        // Compare the elements of lub
        if (lub[0] >= other.lub[2] && lub[1] >= other.lub[3]) {
            if (!(lub[0] == other.lub[2] && lub[1] == other.lub[3])) {
                return true;
            }
        }
        return false;
    }
    array<int, 4> lub;
    vector<int> path;
};

vector<array<int, 2>> readFragementIndex(const char* fragmentIndexFilename){
    ifstream fin(fragmentIndexFilename);
    int fragmentID, nodeID;
    // For each node: [Fragment ID, the corresponding nodeID in the Fragment]
    vector<array<int, 2>> fragmentIndex;
    array<int, 2> nodeIndex;
    // load fin and store data into vector
    while(fin >> fragmentID >> nodeID ){
        nodeIndex[0] = fragmentID;
        nodeIndex[1] = nodeID;
        fragmentIndex.push_back(nodeIndex);
    }
    fin.close();
    return fragmentIndex;
}


vector<vector<int>> readBoundaryNode(const char* boundaryNodeFileName){
    vector<vector<int>> boundaryNodes;
    ifstream inputFile(boundaryNodeFileName);
    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            vector<int> row;
            stringstream ss(line);
            int nodeID;
            while (ss >> nodeID) {
                row.push_back(nodeID);
            }
            boundaryNodes.push_back(row);
        }
        inputFile.close();

    } else {
        std::cout << "Failed to open the file." << std::endl;
    }
    return boundaryNodes;
}

vector<BoundaryPath> onePairBoundaryPathOf(int snode, int dnode, int sBN, int dBN){
    
    vector<BoundaryPath> onePairBoundaryPath;
    return onePairBoundaryPath;
}


vector<BoundaryPath> boundaryPathDominanceCheck(vector<BoundaryPath> boundaryPathSet){
    // for each boundaryPath, the first four elements are (LB1, LB2, UB1, UB2), respectively
    vector<BoundaryPath> paretoBoundaryPath;
    return paretoBoundaryPath;
}

vector<vector<int>> paretoBoundaryPathBetween(int snode, int dnode){
    vector<BoundaryPath> boundaryPathSet;
    vector<BoundaryPath> paretoBoundaryPathSet;
    int sBN, dBN;
    vector<array<int, 2>> fragmentIndex = readFragementIndex("../b3hepv/fragmentIndex.txt");
    int sfragment = fragmentIndex[snode-1][0];
    int dfragment = fragmentIndex[dnode-1][0];
    vector<vector<int>> boundaryNodeSet = readBoundaryNode("../b3hepv/boundaryNodes.txt");
    vector<int> sBoundaryNode = boundaryNodeSet[sfragment];
    vector<int> dBoundaryNode = boundaryNodeSet[dfragment];
    
    for (size_t i = 0; i < sBoundaryNode.size(); ++i) {
        sBN = sBoundaryNode[i];
        for (size_t j = 0; j < sBoundaryNode.size(); ++j){
            dBN = dBoundaryNode[j];
            vector<BoundaryPath> onePairBoundaryPathSet = onePairBoundaryPathOf(snode, dnode, sBN, dBN); 
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
    vector<vector<int>> boundaryPathSet = paretoBoundaryPathBetween(snode, dnode);
    vector<array<int, 2>> superParetoCostSet = expandPathForBoundaryPathSet(boundaryPathSet);
    vector<array<int, 2>> solutions = dominanceCheck(superParetoCostSet); 
    int nsolutions = solutions.size();
    return nsolutions;
}

int boaPathRetrieval(int snode, int dnode, const char* filename) {
    int nsolutions = 0;
    unsigned (*solutions)[2] = paretoPathsInFragment(snode, dnode, filename);
    // Access and print the values
    for (int i = 0; i < MAX_SOLUTIONS; i++) {
        if (solutions[i][0] >0)
            nsolutions+=1;
    }
    return nsolutions;
}


// int main(){
//     testHbor();
//     return 0;
// }

