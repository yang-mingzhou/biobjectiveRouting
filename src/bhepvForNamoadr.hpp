#ifndef BHEPVNAMOADR_H
#define BHEPVNAMOADR_H

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
#include "namoadrPathRetrieval.h"

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


class HBORBasic {
public:
    HBORBasic(const std::string& map, int npar);

    void load();
//     int hbor(int snode, int dnode);
//     int boa(int snode, int dnode);
    int namor(int snode, int dnode);
    void freeGraphDataVector();

public:
    std::string mapName;
    int nPartitions;
    std::string fileFolderName;
    std::vector<GraphData> graphDataVector;
    std::vector<std::vector<int>> adjacencyList;
    std::vector<std::vector<int>> partitions;
    std::vector<std::vector<int>> fragmentIndex;
    std::vector<std::vector<int>> boundaryNodeSet;
    std::map<int, std::map<int, std::vector<std::vector<int>>>> fragmentEncodedPathView;
    std::map<int, std::map<int, std::vector<std::vector<int>>>> boundaryEncodedPathView;
    int numVertices;

    void loadEncodedPathView();
    void loadBoundaryNodes();
    void loadFragments();
    void readOriginGraph();
    void readOriginGraphDataFromFile(GraphData* graphData);
    void loadFragmentIndex();
    std::vector<BiobjectivePath> onePairBoundaryPathOf(int snode, int dnode, int sBN, int dBN);
    std::vector<BiobjectivePath> generateCombinations(const std::vector<BiobjectivePath>& vec1, const std::vector<BiobjectivePath>& vec2, const std::vector<BiobjectivePath>& vec3);
    std::vector<BiobjectivePath> dominanceCheck(std::vector<BiobjectivePath> superParetoCostSet);
    void readGraphDataFromFile(GraphData* graphData, const std::string& filename);
    void cleanupGraphDataCpp(GraphData* graphData);
    std::vector<BiobjectivePath> namorPathRetrievalWithInFragment(int snode, int dnode, int fragmentId);
};


#endif  // BIOBJECTIVE_GRAPH_H
