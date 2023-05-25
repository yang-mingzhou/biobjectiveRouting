#ifndef HBOR_H
#define HBOR_H

using namespace std;

// Function declaration
vector<array<int, 2>> readFragementIndex(const char* fragmentIndexFilename);
vector<vector<int>> readBoundaryNode(const char* boundaryNodeFileName);
vector<BoundaryPath> onePairBoundaryPathOf(int snode, int dnode, int sBN, int dBN);
vector<vector<int>> boundaryPathDominanceCheck(vector<vector<int>> boundaryPathSet);
vector<vector<int>> paretoBoundaryPathBetween(int snode, int dnode);
vector<array<int, 2>> expandPathCostOf(vector<int> boundaryPath);
vector<array<int, 2>> expandPathForBoundaryPathSet(vector<vector<int>> boundaryPathSet);
vector<array<int, 2>> dominanceCheck(vector<array<int, 2>> superParetoCostSet);
int hbor(int snode, int dnode, const char* filename);
int boaPathRetrieval(int snode, int dnode, const char* filename);
#endif
