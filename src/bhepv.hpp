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
#include "bodPathRetrieval.h"
#include "bod.h"
struct Edge {
    int end;
    int cost1;
    int cost2;
    Edge(int _end, int _cost1, int _cost2) : end(_end), cost1(_cost1), cost2(_cost2) {}
};




class BHEPV{
    public:
    std::string fileFolderName;
    std::string mapName;
    int nPartitions; // number of partitions
    int numVertices; // number of numVertices
    std::vector<int> partitions;  // Vector to store partition information
    std::vector<GraphData> graphDataVector;
    std::vector<std::array<int, 2>> fragmentIndex;// For each node: [Fragment ID, the corresponding nodeID in the Fragment]
    std::unordered_set<int> boundaryNodes; // set of boundary nodesIndex (start from 0)
    std::vector<std::vector<int>> boundaryNodeSet;  // i: Fragments j: boundary nodes id
    std::vector<std::vector<Edge>> adjacencyList;
    std::unordered_map<int, std::unordered_map<int, std::vector<std::vector<int>>>> fragmentEncodedPathView;
    std::vector<std::vector<int>> boundaryGraph;
    std::unordered_map<int, std::unordered_map<int, std::vector<std::vector<int>>>> boundaryEncodedPathView;
    std::unordered_map<int, std::unordered_map<int, int>> fragmentIdToOriginId; // fragment:locaID -> globalID
    std::unordered_map<int,int> boundaryIdMap;
    
    BHEPV(const std::string& map, int npar);
    
    bool isBoundaryNode(int vertex) const {
        return boundaryNodes.count(vertex) > 0;
    }
    
    void addEdge(int start, int end, int cost1, int cost2);
    void readOriginGraph();
    void readOriginGraphDataFromFile(GraphData* graphData) ;
    void readPartition() ;
    void updateBoundaryNodes();
    void encodedPathViewToJson(nlohmann::json& j, const std::unordered_map<int, std::unordered_map<int, std::vector<std::vector<int>>>>& data);
    void saveEncodedPathView();    
    void loadEncodedPathView();    
    void saveBoundaryNodePartition();    
    void generateAndSaveFragments();
    void loadBoundaryNodes();
    void loadFragments() ;
    void readGraphDataFromFile(GraphData* graphData, const std::string& filename);
    void saveBoundaryGraph();
    void bodPathRetrievalForFragmentEncodedPathView(int snode, int fragmentId);
    void encodeFragmentPathView();
    void encodeBoundaryPathView();
    void loadFragmentIndex();
    void printFragmentEncodedPathView() ;
    void PrecomputationAndSave();
    void load();
    void freeGraphDataVector();
};



#endif  // BIOBJECTIVE_GRAPH_H
