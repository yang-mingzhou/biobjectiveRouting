#ifndef BIOBJECTIVE_GRAPH_H
#define BIOBJECTIVE_GRAPH_H

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

#ifndef EDGE
#define EDGE
struct Edge {
    int end;
    int cost1;
    int cost2;
    Edge(int _end, int _cost1, int _cost2) : end(_end), cost1(_cost1), cost2(_cost2) {}
};
#endif

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


#endif  // BIOBJECTIVE_GRAPH_H
