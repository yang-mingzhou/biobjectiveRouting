#ifndef HBOR_H
#define HBOR_H
#include <unordered_map>
using namespace std;

class BoundaryPath {
    // for each boundaryPath, the lub: four elements are (LB1, LB2, UB1, UB2); path: a sequence of boundary nodes
    public:
    // Constructor
    BoundaryPath() = default;
    BoundaryPath(const vector<int>& lub_values, const vector<int>& path_values)
        : lub(lub_values), path(path_values) {
        // Ensure the lubs vector has exactly 4 elements
        if (lub_values.size() != 4) {
            throw runtime_error("Invalid number of lubs values. Expected 4 elements.");
        }
    }

    bool isDominatedBy(const BoundaryPath& other) const;
    void printPath() const;
    bool isPreceeding(const BoundaryPath& other) const;
    BoundaryPath concatPaths(const BoundaryPath& other) const;

    vector<int> lub; // Contains 4 elements
    vector<int> path;
};

class B3HEPV {
    // for each boundaryPath, the lub: four elements are (LB1, LB2, UB1, UB2); path: a sequence of boundary nodes
    public:
    string fileFolderName;
    vector<array<int, 2>> fragmentIndex;
    vector<vector<int>> boundaryNodeSet;
    unordered_map<int, unordered_map<int, vector<BoundaryPath>>> boundaryEncodedPathView;
    unordered_map<int, unordered_map<int, BoundaryPath>> fragmentEncodedPathView;
    // Other member variables and functions...

    B3HEPV(const string& folderName);
    
    vector<BoundaryPath> onePairBoundaryPathOf(int snode, int dnode, int sBN, int dBN);
    vector<BoundaryPath> boundaryPathDominanceCheck(vector<BoundaryPath> boundaryPathSet);
    vector<BoundaryPath> paretoBoundaryPathBetween(int snode, int dnode);
    vector<array<int, 2>> expandPathCostOf(BoundaryPath boundaryPath);
    vector<array<int, 2>> pathRetrievalWithInFragment(int snode, int dnode, int fragmentId);
    vector<array<int, 2>> expandPathForBoundaryPathSet(vector<BoundaryPath> boundaryPathSet);
    vector<array<int, 2>> dominanceCheck(vector<array<int, 2>> superParetoCostSet);
    int hbor(int snode, int dnode);
    int boaPathRetrieval(int snode, int dnode, const string& filename);
};



#endif
