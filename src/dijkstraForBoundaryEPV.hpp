#ifndef DIJFORBEPV_H
#define DIJFORBEPV_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include <fstream>
#include <queue>
#include <climits>
#include "json.hpp"
#include "boundaryPath.hpp"

using namespace std;
using json = nlohmann::json;



class Bi_objective_Search {
public:
    Bi_objective_Search(const string& foldername);

    std::vector<std::vector<BoundaryPath>> DijkstraTraverse(int start);
    void allPairs();
    void save_all_path(string filetype);
    void convert_map_of_map_to_json_file();

private:
    unordered_map<int, unordered_map<int, vector<int>>> adjNodes, existingLUBs;
    vector<vector<int>> allpath;
    vector<int> key_nodes;
    std::map<int, int> nodeToIndex;
    string folderpath;
    std::unordered_map<int, int> nodePartitionMap;
    int current_s_node;
    vector<vector<vector<BoundaryPath>>> preComputedPaths;
    
    vector<vector<int>> fileToVector(const string& filename);
    unordered_map<int, unordered_map<int, vector<int>>> createAdjNodes(vector< vector<int> > mapData);
    bool dominates(const vector<int>& a, const vector<int>& b);
};


#endif  // DIJFORBEPV_H