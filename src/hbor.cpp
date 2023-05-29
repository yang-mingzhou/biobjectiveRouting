#include <iostream>
#include <vector> 
#include <fstream>
#include <sstream>
#include <list>
#include <array>
#include <unordered_map>
#include <time.h>
#include <algorithm>
#include <string>
#include "pathRetrieval.h"
#include "hbor.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;



// module load gcc/11.3.0
// gcc -std=c99 -o main.o hbor.cpp pathRetrieval.c heap.c boastar.c graph.c -lstdc++

bool BoundaryPath::isDominatedBy(const BoundaryPath& other) const {
    // Compare the elements of lub
    if (lub[0] >= other.lub[2] && lub[1] >= other.lub[3]) {
        if (!(lub[0] == other.lub[2] && lub[1] == other.lub[3])) {
            return true;
        }
    }
    return false;
}

void BoundaryPath::printPath() const {
    cout << "Path: ";
    for (const int& node : path) {
        cout << node << ", ";
    }
    cout << endl;
}

bool BoundaryPath::isPreceeding(const BoundaryPath& other) const {
    if (!path.empty() && !other.path.empty()) {
        return path.back() == other.path.front();
    }
    return false;
}

BoundaryPath BoundaryPath::concatPaths(const BoundaryPath& other) const {
        if (isPreceeding(other)) {
            vector<int> newLub(4);
            vector<int> newPath;

            // Calculate the sum of corresponding lub values
            for (int i = 0; i < 4; ++i) {
                newLub[i] = lub[i] + other.lub[i];
            }

            // Append the path elements from both paths, removing duplicates
            newPath = path;
            newPath.insert(newPath.end(), other.path.begin() + 1, other.path.end());

            return BoundaryPath(newLub, newPath);
        }

        throw runtime_error("Cannot concatenate paths. The paths are not preceeding each other.");
    }

B3HEPV::B3HEPV(const std::string& folderName) : fileFolderName(folderName) {
    // read fragment index
    string fragmentIndexFilename =  folderName + "/fragmentIndex.txt";
    ifstream fin(fragmentIndexFilename);
    int fragmentID, nodeID;
    // For each node: [Fragment ID, the corresponding nodeID in the Fragment]
    array<int, 2> nodeIndex;
    // load fin and store data into vector
    while(fin >> fragmentID >> nodeID ){
        nodeIndex[0] = fragmentID;
        nodeIndex[1] = nodeID;
        fragmentIndex.push_back(nodeIndex);
    }
    fin.close();

    // read boundary nodes
    string boundaryNodeFileName =  folderName + "/boundaryNodes.txt";
    ifstream inputFile(boundaryNodeFileName);
    string line;
    while (getline(inputFile, line)) {
        vector<int> row;
        stringstream ss(line);
        int nodeID;
        while (ss >> nodeID) {
            row.push_back(nodeID);
        }
        boundaryNodeSet.push_back(row);
    }
    inputFile.close();

    // read boundary encoded path view
    // {snode: {dnoded:[[lb1,lb2,ub1,ub2,path1],[lb1',lb2',ub1',ub2',path2], ...]}}
    string boundaryEncodedPathFileName =  folderName + "/boundaryEncodedPathView.json";
    ifstream file(boundaryEncodedPathFileName);
    string jsonString((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    // Parse the JSON string
    json jsonData;
    try {
        jsonData = json::parse(jsonString);
    } catch (json::parse_error& e) {
        cerr << "Failed to parse the JSON file: " << e.what() << endl;
    }
    // Access the JSON data and populate the data structure
    if (jsonData.is_object()) {
        for (const auto& entry1 : jsonData.items()) {
            int snode = stoi(entry1.key());

            if (entry1.value().is_object()) {
                for (const auto& entry2 : entry1.value().items()) {
                    int dnode = stoi(entry2.key());

                    if (entry2.value().is_array()) {
                        vector<BoundaryPath> pathSet;
                        for (const auto& arrayValue : entry2.value()) {
                            if (arrayValue.is_array()) {
                                BoundaryPath path;
                                for (size_t i = 0; i < arrayValue.size(); i++) {
                                    if (arrayValue[i].is_number()) {
                                        if (i < 4) {
                                            path.lub.push_back(arrayValue[i]);
                                        } else {
                                            path.path.push_back(arrayValue[i]);
                                        }
                                    }
                                }
                                pathSet.push_back(path);
                            }
                        }
                        boundaryEncodedPathView[snode][dnode] = pathSet;
                    }
                }
            }
        }
    }

    // read fragment encoded path view
    string fragmentEncodedPathFileName =  folderName + "/encodedFragmentPathView.json";
    fstream fragementFile(fragmentEncodedPathFileName);

    json jsonFragmentData;
    fragementFile >> jsonFragmentData;

    for (const auto& entry : jsonFragmentData.items()) {
        int key1 = stoi(entry.key());

        for (const auto& innerEntry : entry.value().items()) {
            int key2 = stoi(innerEntry.key());

            const json& lubsJson = innerEntry.value();
            if (!lubsJson.is_array() || lubsJson.size() != 4) {
                throw runtime_error("Invalid lubs format. Expected an array of 4 elements.");
            }

            vector<int> lubs = lubsJson.get<vector<int>>();

            BoundaryPath boundaryPath(lubs, {key1, key2});

            fragmentEncodedPathView[key1][key2] = boundaryPath;
        }
    }
    fragementFile.close();
}


vector<BoundaryPath> B3HEPV::onePairBoundaryPathOf(int snode, int dnode, int sBN, int dBN){
    vector<BoundaryPath> onePairBoundaryPathSet;
    BoundaryPath headPath,tailPath, onePairPath;


    if (snode == sBN){
        headPath = BoundaryPath({0, 0, 0, 0}, {snode});
    }
    else {
        headPath = fragmentEncodedPathView.at(snode).at(sBN);
    }

    if (dnode == dBN){
        tailPath = BoundaryPath({0, 0, 0, 0}, {dnode});
    }
    else {
        tailPath = fragmentEncodedPathView.at(dBN).at(dnode);
    }
    vector<BoundaryPath> interPathSetBetweenSbnAndDbn = boundaryEncodedPathView.at(sBN).at(dBN);
    for (const auto& interPath : interPathSetBetweenSbnAndDbn) {
        onePairPath = headPath.concatPaths(interPath).concatPaths(tailPath);
        onePairBoundaryPathSet.push_back(onePairPath);
    }
    return onePairBoundaryPathSet;
}

vector<BoundaryPath> B3HEPV::boundaryPathDominanceCheck(vector<BoundaryPath> boundaryPathSet){
    // for each boundaryPath, the first four elements are (LB1, LB2, UB1, UB2), respectively
    vector<BoundaryPath> paretoBoundaryPath;
    BoundaryPath currentPath, comparedPath;
    vector<int> paretoIndex(boundaryPathSet.size(), 1);
    for (size_t i = 0; i < boundaryPathSet.size(); ++i) {
        if (paretoIndex[i]==1){
            currentPath = boundaryPathSet[i];
            for (size_t j = i+1; j < boundaryPathSet.size(); ++j){
                comparedPath = boundaryPathSet[j];
                if (currentPath.isDominatedBy(comparedPath)){
                    paretoIndex[i]=0;
                }
                if (comparedPath.isDominatedBy(currentPath)){
                    paretoIndex[j]=0;
                }
            }   
        }
        if (paretoIndex[i]==1){
            paretoBoundaryPath.push_back(currentPath);
        }
    }
    return paretoBoundaryPath;
}

vector<BoundaryPath> B3HEPV::paretoBoundaryPathBetween(int snode, int dnode){
    vector<BoundaryPath> boundaryPathSet;
    vector<BoundaryPath> paretoBoundaryPathSet;
    int sBN, dBN;
    int sfragment = fragmentIndex[snode-1][0];
    int dfragment = fragmentIndex[dnode-1][0];
    vector<int> sBoundaryNode = boundaryNodeSet[sfragment];
    vector<int> dBoundaryNode = boundaryNodeSet[dfragment];
    
    for (size_t i = 0; i < sBoundaryNode.size(); ++i) {
        sBN = sBoundaryNode[i];
        for (size_t j = 0; j < sBoundaryNode.size(); ++j){
            dBN = dBoundaryNode[j];
            vector<BoundaryPath> onePairBoundaryPathSet = onePairBoundaryPathOf(snode, dnode, sBN, dBN); 
            boundaryPathSet.insert(boundaryPathSet.end(), onePairBoundaryPathSet.begin(), onePairBoundaryPathSet.end());
        }
    }
    paretoBoundaryPathSet = boundaryPathDominanceCheck(boundaryPathSet);
    return paretoBoundaryPathSet;
}

vector<array<int, 2>> B3HEPV::expandPathCostOf(BoundaryPath boundaryPath){
    vector<array<int, 2>> pathCostSet;
    return pathCostSet;
}

vector<array<int, 2>> B3HEPV::pathRetrievalWithInFragment(int snode, int dnode, int fragmentId) {
    vector<array<int, 2>> pathCostSet;
    return pathCostSet;
}



vector<array<int, 2>> B3HEPV::expandPathForBoundaryPathSet(vector<BoundaryPath> boundaryPathSet){
    vector<array<int, 2>> expendedPathCostSet;
    for (size_t i = 0; i < boundaryPathSet.size(); ++i) {
        vector<array<int, 2>> pathCostWithOneBoundaryPath = expandPathCostOf(boundaryPathSet[i]);
        expendedPathCostSet.insert(expendedPathCostSet.end(), pathCostWithOneBoundaryPath.begin(), pathCostWithOneBoundaryPath.end());
    }
    return expendedPathCostSet;
}   

vector<array<int, 2>> B3HEPV::dominanceCheck(vector<array<int, 2>> superParetoCostSet){
    vector<array<int, 2>> paretoPathCostSet;
    return paretoPathCostSet;
}


int B3HEPV::hbor(int snode, int dnode){
    vector<BoundaryPath> boundaryPathSet = paretoBoundaryPathBetween(snode, dnode);
    vector<array<int, 2>> superParetoCostSet = expandPathForBoundaryPathSet(boundaryPathSet);
    vector<array<int, 2>> solutions = dominanceCheck(superParetoCostSet); 
    int nsolutions = solutions.size();
    return nsolutions;
}

int B3HEPV::boaPathRetrieval(int snode, int dnode, const string& filename) {
    // convert filename to char *
    int nsolutions = 0;
    const char* charFilename = filename.c_str();
    unsigned (*solutions)[2] = paretoPathsInFragment(snode, dnode, charFilename);
    // Access and print the values
    for (int i = 0; i < MAX_SOLUTIONS; i++) {
        if (solutions[i][0] >0)
            nsolutions+=1;
    }
    return nsolutions;
}


// int main(){
//     testHbor();
//     return 0;
// }

