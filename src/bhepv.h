#ifndef BHEPV_H
#define BHEPV_H

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


class BHEPV{
    public:
    std::string fileFolderName;
    std::string mapName;
    int nPartitions;
    int numVertices;
    std::vector<int> partitions;  // Vector to store partition information
    std::vector<std::vector<int>> boundaryNodeSet;
    std::vector<GraphData> graphDataVector;
    std::unordered_set<int> boundaryNodes;
    
    
    BHEPV(const std::string& map, int npar);
}



#if 0
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
};


class BiobjectiveGraph {
public:
    int numVertices;
    std::vector<std::vector<Edge>> adjacencyList;
    std::vector<int> partitions;  // Vector to store partition information
    int partitionCount;
    std::unordered_set<int> boundaryNodes;

public:
    
    BiobjectiveGraph(int numVertices)
        : numVertices(numVertices), adjacencyList(numVertices), partitions(numVertices) {}
    BiobjectiveGraph(const std::string& filename);
    void addVertex();
    void addEdge(int start, int end, int cost1, int cost2);
    void readPartition(const std::string& partitionFile);
    std::unordered_set<int> findBoundaryNodes() const;
    void updateBoundaryNodes() {
        boundaryNodes = findBoundaryNodes();
    }
    bool isBoundaryNode(int vertex) const {
        return boundaryNodes.count(vertex) > 0;
    }
    std::pair<std::vector<int>, std::vector<int>> oneToAllDijkstra(int source, int costComponent) const;
    
    void saveFragments(const std::string& outputFolderName) const;
    void saveExistingLUB(const std::string& inputFolderName) const;
};
#endif

#endif  // BIOBJECTIVE_GRAPH_H
