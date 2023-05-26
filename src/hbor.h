#ifndef HBOR_H
#define HBOR_H

using namespace std;

// Function declaration
class BoundaryPath {
    // for each boundaryPath, the lub: four elements are (LB1, LB2, UB1, UB2); path: a sequence of boundary nodes
    public:
    BoundaryPath(const array<int, 4>& bounds, const vector<int>& pathSequence);

    bool isDominatedBy(const BoundaryPath& other) const;
    array<int, 4> lub;
    vector<int> path;
};

vector<array<int, 2>> readFragementIndex(const char* fragmentIndexFilename);
vector<vector<int>> readBoundaryNode(const char* boundaryNodeFileName);
vector<BoundaryPath> onePairBoundaryPathOf(int snode, int dnode, int sBN, int dBN);
vector<BoundaryPath> boundaryPathDominanceCheck(vector<BoundaryPath> boundaryPathSet);
vector<BoundaryPath> paretoBoundaryPathBetween(int snode, int dnode);
vector<array<int, 2>> expandPathCostOf(vector<int> boundaryPath);
vector<array<int, 2>> expandPathForBoundaryPathSet(vector<vector<int>> boundaryPathSet);
vector<array<int, 2>> dominanceCheck(vector<array<int, 2>> superParetoCostSet);
int hbor(int snode, int dnode, const char* filename);
int boaPathRetrieval(int snode, int dnode, const char* filename);
#endif
