#ifndef HBOR_H
#define HBOR_H

using namespace std;

class BoundaryPath {
    // for each boundaryPath, the lub: four elements are (LB1, LB2, UB1, UB2); path: a sequence of boundary nodes
    public:
    BoundaryPath(){}

    BoundaryPath(const array<int, 4>& bounds, const vector<int>& pathSequence)
        : lub(bounds), path(pathSequence) {}

    bool isDominatedBy(const BoundaryPath& other) const;

    void printPath() const;

    array<int, 4> lub;
    vector<int> path;
};

class B3HEPV {
    // for each boundaryPath, the lub: four elements are (LB1, LB2, UB1, UB2); path: a sequence of boundary nodes
    public:
    string fileFolderName;
    vector<array<int, 2>> fragmentIndex;
    vector<vector<int>> boundaryNodeSet;
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
