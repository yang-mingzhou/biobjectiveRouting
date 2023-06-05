#ifndef HBOR_H
#define HBOR_H
#include <unordered_map>
#include <string>
#include "pathRetrieval.h"

constexpr int MAXNODES = 4000000;
constexpr int MAXNEIGH = 45;


class BoundaryPath {
    // for each boundaryPath, the lub: four elements are (LB1, LB2, UB1, UB2); path: a sequence of boundary nodes
    public:
    // Constructor
    BoundaryPath() = default;
    BoundaryPath(const std::vector<int>& lub_values, const std::vector<int>& path_values)
        : lub(lub_values), path(path_values) {
        // Ensure the lubs vector has exactly 4 elements
        if (lub_values.size() != 4) {
            throw std::runtime_error("Invalid number of lubs values. Expected 4 elements.");
        }
    }

    int startNode() const;
    int endNode() const;
    int getNode(int i) const;
    int len() const {
        return path.size();
    }
    bool isDominatedBy(const BoundaryPath& other) const;
    void printPath() const;
    bool isPreceeding(const BoundaryPath& other) const;
    BoundaryPath concatWith(const BoundaryPath& other) const;
    BoundaryPath reverse() const {
        std::vector<int> reversed_path(path.rbegin(), path.rend());
        return BoundaryPath(lub, reversed_path);
    }
    
    bool eq(const BoundaryPath& other) const {
        return path == other.path;
    }

    std::vector<int> lub; // Contains 4 elements
    std::vector<int> path;

};


class Solution {
public:
    int cost1;
    int cost2;

    Solution() = default;
    Solution(int c1, int c2) : cost1(c1), cost2(c2) {}

    bool isDominatedBy(const Solution& other) const {
//         return cost1 >= other.cost1 && cost2 >= other.cost2 && (cost1 > other.cost1 || cost2 > other.cost2);
        return cost1 >= other.cost1 && cost2 >= other.cost2; // assume no pareto paths with same cost.
    }

    Solution concatWith(const Solution& other) const {
        int newCost1 = cost1 + other.cost1;
        int newCost2 = cost2 + other.cost2;
        return Solution(newCost1, newCost2);
    }
};


class B3HEPV {
    // for each boundaryPath, the lub: four elements are (LB1, LB2, UB1, UB2); path: a sequence of boundary nodes
    public:
    std::string fileFolderName;
    std::string mapName;
    int nPartitions;
//     int nPartitions;
    std::vector<std::array<int, 2>> fragmentIndex;// For each node: [Fragment ID, the corresponding nodeID in the Fragment]
    std::vector<std::vector<int>> boundaryNodeSet;
    std::unordered_map<int, std::unordered_map<int, std::vector<BoundaryPath>>> boundaryEncodedPathView;
    std::unordered_map<int, std::unordered_map<int, BoundaryPath>> fragmentEncodedPathView;
    std::unordered_map<int, std::unordered_map<int, std::vector<int>>> adjacentLub;
    std::vector<GraphData> graphDataVector;
    // Other member variables and functions...

    B3HEPV(const std::string& map, int npar);
    
    std::vector<BoundaryPath> onePairBoundaryPathOf(int snode, int dnode, int sBN, int dBN);
    std::vector<BoundaryPath> boundaryPathDominanceCheck(std::vector<BoundaryPath> boundaryPathSet);
    std::vector<BoundaryPath> paretoBoundaryPathBetween(int snode, int dnode);
    std::vector<Solution> expandPathCostOf(BoundaryPath boundaryPath);
    std::vector<Solution> pathRetrievalWithInFragment(int snode, int dnode, int fragmentId);
    std::vector<Solution> expandPathForBoundaryPathSet(std::vector<BoundaryPath> boundaryPathSet);
    std::vector<Solution> dominanceCheck(std::vector<Solution> superParetoCostSet);
    std::vector<Solution> combineCostSet(std::vector<Solution> costSet1, std::vector<Solution> costSet2);
    int hbor(int snode, int dnode);
    int boaPathRetrieval(int snode, int dnode);
    std::vector<BoundaryPath> reversePaths(const std::vector<BoundaryPath>& paths);
    void read_adjacent_table();
    int boaPathRetrievalFromFile(int snode, int dnode, const std::string& filename);
    void readDataFromFile(GraphData* graphData, const std::string& filename); 
    void cleanupGraphDataVector();
};

void precomputation(const std::string & mapName);

#endif
