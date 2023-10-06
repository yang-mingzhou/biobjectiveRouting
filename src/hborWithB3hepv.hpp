#ifndef HBOR_B3HEPV_H
#define HBOR_B3HEPV_H

#include <vector>
#include <queue>
#include <limits>
#include <tuple>
#include <string>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <queue>
#include <functional>
#include <unordered_set>
#include <iomanip>
#include "json.hpp"
#include "pathRetrieval.h"
#include "namoadrPathRetrieval.h"
#include "boastar.h"

class BiobjectivePath {
public:
    int cost1;
    int cost2;

    BiobjectivePath() = default;
    BiobjectivePath(int c1, int c2) : cost1(c1), cost2(c2) {}

    bool isDominatedBy(const BiobjectivePath& other) const {
        return cost1 >= other.cost1 && cost2 >= other.cost2 && (cost1 > other.cost1 || cost2 > other.cost2);
    }

    bool eq(const BiobjectivePath& other) const{
        return cost1 == other.cost1 && cost2 == other.cost2;
    }

    BiobjectivePath concatWith(const BiobjectivePath& other) const {
        int newCost1 = cost1 + other.cost1;
        int newCost2 = cost2 + other.cost2;
        return BiobjectivePath(newCost1, newCost2);
    }
};


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
    void printLub() const;
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


// Comparison functor for the min-heap.
struct BoundaryPathCompare {
    bool operator()(const BoundaryPath& a, const BoundaryPath& b) const {
        if (a.lub[0] == b.lub[0]) {
            return a.lub[1] > b.lub[1];  // Note the change to '>' to make it a min-heap.
        }
        return a.lub[0] > b.lub[0];
    }
};



class B3HBORBasic {
public:
    B3HBORBasic(const std::string& map, int npar);

    void load();
    int hbor(int snode, int dnode);
    int boa(int snode, int dnode);
//     int namor(int snode, int dnode);
    void freeGraphDataVector();

private:
    std::string mapName;
    int nPartitions;
    std::string fileFolderName;
    std::vector<GraphData> graphDataVector;
    std::vector<std::vector<int>> adjacencyList;
    std::vector<std::vector<int>> partitions;
    std::vector<std::vector<int>> fragmentIndex;
    std::vector<std::vector<int>> boundaryNodeSet;
    std::map<int, std::map<int, std::vector<std::vector<int>>>> fragmentEncodedPathView;
//     std::map<int, std::map<int, std::vector<std::vector<int>>>> boundaryEncodedPathView;
    std::unordered_map<int, std::unordered_map<int, std::vector<BoundaryPath>>> b3boundaryEPV;
    std::unordered_map<int, std::unordered_map<int, BoundaryPath>> b3fragmentEPV;
    std::unordered_map<int, std::unordered_map<int, std::vector<int>>> adjacentLub;
    
    int numVertices;
    
    int numberOfExpendedEdges;
     std::unordered_map<int, std::unordered_map<int, std::vector<std::pair<int, int>>>> sortedBoundaryPairsMap;


    void loadEncodedPathView();
    void loadBoundaryNodes();
    void loadFragments();
    void readOriginGraph();
    void readOriginGraphDataFromFile(GraphData* graphData);
    void loadFragmentIndex();
    
    void populateSortedBoundaryPairs();

//     std::vector<BiobjectivePath> onePairBoundaryPathOf(int snode, int dnode, int sBN, int dBN);
    std::vector<BiobjectivePath> generateCombinations(const std::vector<BiobjectivePath>& vec1, const std::vector<BiobjectivePath>& vec2, const std::vector<BiobjectivePath>& vec3);
    std::vector<BiobjectivePath> dominanceCheck(std::vector<BiobjectivePath> superParetoCostSet);
    std::vector<BiobjectivePath> boaPathRetrievalWithInFragment(int snode, int dnode, int fragmentId);
    void readGraphDataFromFile(GraphData* graphData, const std::string& filename);
    void cleanupGraphDataCpp(GraphData* graphData);
    // new function for using B3boundaryEPV
    std::vector<BiobjectivePath> expandPathCostOf(BoundaryPath boundaryPath);
    std::vector<BiobjectivePath> combineCostSet(std::vector<BiobjectivePath> costSet1, std::vector<BiobjectivePath> costSet2);
    std::vector<BiobjectivePath> pathRetrievalWithInFragment(int snode, int dnode);
//     std::vector<BoundaryPath> onePairB3PathOf(int snode, int dnode, int sBN, int dBN);
    std::vector<BoundaryPath> reversePaths(const std::vector<BoundaryPath>& paths);
    std::vector<BoundaryPath> boundaryPathDominanceCheck(std::vector<BoundaryPath> boundaryPathSet);
    std::vector<BiobjectivePath> expandPathForBoundaryPathSet(std::vector<BoundaryPath> boundBasedBoundaryPathSet);
    std::vector<BoundaryPath> paretoBoundaryPathBetween(int snode, int dnode);
    std::vector<BoundaryPath> onePairB3PathOf(int snode, int dnode, int sBN, int dBN, std::priority_queue<BoundaryPath, std::vector<BoundaryPath>, BoundaryPathCompare>& minHeap);
    bool isDominatedBySmallestInHeap(const BoundaryPath& path, const std::priority_queue<BoundaryPath, std::vector<BoundaryPath>, BoundaryPathCompare>& minHeap);
//     std::vector<BiobjectivePath> namorPathRetrievalWithInFragment(int snode, int dnode, int fragmentId);
};


#endif  // BIOBJECTIVE_GRAPH_H
