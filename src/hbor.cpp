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
#include <climits>
#include <chrono>
#include "hbor.h"
#include "json.hpp"
#include "biobjectiveGraph.h"
using namespace std;
using json = nlohmann::json;



// module load gcc/11.3.0
// gcc -std=c99 -o main.o hbor.cpp pathRetrieval.c heap.c boastar.c graph.c -lstdc++



void printEdgeVectorsCPP(const GraphData* graphData) {
    int num_arcs = graphData->numOfArcs;

    std::cout << "Edge Vectors:" << std::endl;
    for (int i = 0; i < num_arcs; i++) {
        int ori = graphData->edgeVectors[i][0];
        int dest = graphData->edgeVectors[i][1];
        int dist = graphData->edgeVectors[i][2];
        int t = graphData->edgeVectors[i][3];
        std::cout << ori << " " << dest << " " << dist << " " << t << std::endl;
    }
}

bool BoundaryPath::isDominatedBy(const BoundaryPath& other) const {
    // Compare the elements of lub
    if (eq(other)) {
        return true;
    }

    if ((lub[0] >= other.lub[0] && lub[1] >= other.lub[3])|| (lub[0] >= other.lub[2] && lub[1] >= other.lub[1])) {
        if (!(lub[0] == other.lub[2] && lub[1] == other.lub[3]) || (lub[0] != lub[2])|| (lub[1] != lub[3])|| (other.lub[0] != other.lub[2])|| (other.lub[1] != other.lub[3]) ) {
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

void BoundaryPath::printLub() const {
    cout << "LUB: ";
    for (const int& i : lub) {
        cout << i << ", ";
    }
    cout << endl;
}

bool BoundaryPath::isPreceeding(const BoundaryPath& other) const {
    if (!path.empty() && !other.path.empty()) {
        return path.back() == other.path.front();
    }
    return false;
}

int BoundaryPath::startNode() const {
    if (!path.empty()) {
        return path.front();
    } else {
        // Handle the case where the path is empty
        return -1;
    }
}

int BoundaryPath::endNode() const {
    if (!path.empty()) {
        return path.back();
    } else {
        // Handle the case where the path is empty
        return -1;
    }
}

int BoundaryPath::getNode(int i) const {
    if (i >= 0 && i < path.size()) {
        return path[i];
    } else {
        // Handle the case where the index is out of range
        return -1;
    }
}


// BoundaryPath BoundaryPath::concatWith(const BoundaryPath& other) const {
//     if (isPreceeding(other)) {
//         vector<int> newLub(4);
//         vector<int> newPath;

//         // Calculate the sum of corresponding lub values
//         for (int i = 0; i < 4; ++i) {
//             newLub[i] = lub[i] + other.lub[i];
//         }

//         // Append the path elements from both paths, removing duplicates
//         newPath = path;
//         newPath.insert(newPath.end(), other.path.begin() + 1, other.path.end());

//         return BoundaryPath(newLub, newPath);
//     }
//     throw runtime_error("Cannot concatenate paths. The paths are not preceeding each other.");
// }



BoundaryPath BoundaryPath::concatWith(const BoundaryPath& other) const {
    if (isPreceeding(other)) {
        vector<int> newLub(4);
        vector<int> newPath;

        int maxInt = std::numeric_limits<int>::max();

        // Calculate the sum of corresponding lub values
        for (int i = 0; i < 4; ++i) {
            // Check for potential overflow
            if (lub[i] >= maxInt || other.lub[i] >= maxInt || (lub[i] > 0 && other.lub[i] > maxInt - lub[i])) {
                newLub[i] = maxInt; // If overflow would occur, set to max value
            } else {
                newLub[i] = lub[i] + other.lub[i];
            }
        }

        // Append the path elements from both paths, removing duplicates
        newPath = path;
        newPath.insert(newPath.end(), other.path.begin() + 1, other.path.end());

        return BoundaryPath(newLub, newPath);
    }
    throw runtime_error("Cannot concatenate paths. The paths are not preceeding each other.");
}



B3HEPV::B3HEPV(const std::string& map, int npar) : mapName(map),nPartitions(npar) {
    cout<< "reading" <<endl;
    // read fragment index
    fileFolderName = "../b3hepv/"+mapName;
    string fragmentIndexFilename =  fileFolderName + "/fragmentIndex.txt";
    ifstream fin(fragmentIndexFilename);
    if (!fin.is_open()) {
        throw std::runtime_error("Failed to open file: " + fragmentIndexFilename);
    }
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
    string boundaryNodeFileName =  fileFolderName + "/boundaryNodes.txt";
    ifstream inputFile(boundaryNodeFileName);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Failed to open file: " + boundaryNodeFileName);
    }
    string line;
    while (getline(inputFile, line)) {
        vector<int> row;
        stringstream ss(line);
        int nodeID;
        while (ss >> nodeID) {
                row.push_back(nodeID);
            
        }
        // cout<<"row size"<< row.size() <<endl;
        boundaryNodeSet.push_back(row);
    }
    inputFile.close();

    // read boundary encoded path view
    // {snode: {dnoded:[[lb1,lb2,ub1,ub2,path1],[lb1',lb2',ub1',ub2',path2], ...]}}
    string boundaryEncodedPathFileName =  fileFolderName + "/boundaryEncodedPathView.json";
    ifstream file(boundaryEncodedPathFileName);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + boundaryEncodedPathFileName);
    }
    string jsonString((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    // Parse the JSON string
    json jsonData;
    try {
        jsonData = json::parse(jsonString);
    } catch (json::parse_error& e) {
        cerr << "Failed to parse the JSON file: " << e.what() << endl;
    }
    int cnt_BoundaryPath = 0;
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
                                cnt_BoundaryPath ++;
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
    cout<< "Number of boundary paths: " << cnt_BoundaryPath << endl;
    int cnt_fragmentPath = 0;
    //revise to ".txt" file
    // read fragment encoded path view
    string fragmentEncodedPathFileName =  fileFolderName + "/fragmentEncodedPathView.txt";
    std::ifstream fragementFile(fragmentEncodedPathFileName);
    if (!fragementFile.is_open()) {
        throw std::runtime_error("Failed to open file: " + fragmentEncodedPathFileName);
    }
    std::string lineInfragment;
    while (std::getline(fragementFile, lineInfragment)) {
        std::istringstream iss(lineInfragment);
        int startNode, endNode, cost1, cost2, cost3, cost4;
        if (iss >> startNode >> endNode >> cost1 >> cost2 >> cost3 >> cost4) {
            cnt_fragmentPath ++;
            BoundaryPath path;
            path.path = {startNode, endNode};
            path.lub = {cost1, cost2, cost3, cost4};
            fragmentEncodedPathView[startNode][endNode] = path;
        } 
    }
    fragementFile.close();
    cout<< "Number of fragment paths: " << cnt_fragmentPath << endl;
    // read adjacentLub.json {snode:node:[lb1, lb2, ub1, ub2]}
    string adjacentFilename =  fileFolderName + "/adjacent_LUB.json";
    ifstream adjfile(adjacentFilename);
    

    if (!adjfile.is_open()) {
        std::cout << "Failed to open JSON file." << std::endl;
    }

    // Read the JSON content from the file
    nlohmann::json jsonDataAdj;
    adjfile >> jsonDataAdj;
    
    
    for (auto it = jsonDataAdj.begin(); it != jsonDataAdj.end(); ++it) {
        int key1 = std::stoi(it.key());

        for (auto innerIt = it.value().begin(); innerIt != it.value().end(); ++innerIt) {
            int key2 = std::stoi(innerIt.key());
            const std::vector<int>& values = innerIt.value()[0];

            adjacentLub[key1][key2] = values;
        }
    }

    
    read_adjacent_table();
    cout<< "fragments read: " << graphDataVector.size()<<endl;

}


vector<BoundaryPath> B3HEPV::onePairBoundaryPathOf(int snode, int dnode, int sBN, int dBN) {
    // cout << snode << dnode<<sBN << dBN<<endl;
    vector<BoundaryPath> onePairBoundaryPathSet;
    BoundaryPath headPath, tailPath, onePairPath;

    if (snode == sBN) {
        headPath = BoundaryPath({0, 0, 0, 0}, {snode});
    } else {
        if (fragmentEncodedPathView.count(snode) > 0 && fragmentEncodedPathView.at(snode).count(sBN) > 0) {
            headPath = fragmentEncodedPathView.at(snode).at(sBN);
        } else if (fragmentEncodedPathView.count(sBN) > 0 && fragmentEncodedPathView.at(sBN).count(snode) > 0) {
            headPath = fragmentEncodedPathView.at(sBN).at(snode).reverse();
        } else {
            throw std::runtime_error("Missing entry in fragmentEncodedPathView");
        }
    }

    if (dnode == dBN) {
        tailPath = BoundaryPath({0, 0, 0, 0}, {dnode});
    } else {
        if (fragmentEncodedPathView.count(dBN) > 0 && fragmentEncodedPathView.at(dBN).count(dnode) > 0) {
            tailPath = fragmentEncodedPathView.at(dBN).at(dnode);
        } else if (fragmentEncodedPathView.count(dnode) > 0 && fragmentEncodedPathView.at(dnode).count(dBN) > 0) {
            tailPath = fragmentEncodedPathView.at(dnode).at(dBN).reverse();
        } else {
            throw std::runtime_error("Missing entry in fragmentEncodedPathView");
        }
    }
    
    if (sBN == dBN){
        onePairPath = headPath.concatWith(tailPath);
        onePairBoundaryPathSet.push_back(onePairPath);
        return onePairBoundaryPathSet;

    }
    else if (boundaryEncodedPathView.count(sBN) > 0 && boundaryEncodedPathView.at(sBN).count(dBN) > 0) {
        const vector<BoundaryPath>& interPathSetBetweenSbnAndDbn = boundaryEncodedPathView.at(sBN).at(dBN);
        for (const auto& interPath : interPathSetBetweenSbnAndDbn) {
            onePairPath = headPath.concatWith(interPath).concatWith(tailPath);
            onePairBoundaryPathSet.push_back(onePairPath);
        }
    }
    else if (boundaryEncodedPathView.count(dBN) > 0 && boundaryEncodedPathView.at(dBN).count(sBN) > 0){
        const vector<BoundaryPath>& interPathSetBetweenSbnAndDbn = reversePaths(boundaryEncodedPathView.at(dBN).at(sBN));
        for (const auto& interPath : interPathSetBetweenSbnAndDbn) {
            onePairPath = headPath.concatWith(interPath).concatWith(tailPath);
            onePairBoundaryPathSet.push_back(onePairPath);
        }
    }
    // cout<<onePairBoundaryPathSet.size()<<endl;
    return onePairBoundaryPathSet;
}





std::vector<BoundaryPath> B3HEPV::reversePaths(const std::vector<BoundaryPath>& paths) {
    std::vector<BoundaryPath> reversed_paths;
    reversed_paths.reserve(paths.size()); // Reserve memory for efficiency

    for (const BoundaryPath& path : paths) {
        BoundaryPath reversed_path = path.reverse();
        reversed_paths.push_back(reversed_path);
    }

    return reversed_paths;
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
                else if (comparedPath.isDominatedBy(currentPath)){
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
    
    // cout<< "sfragment: " << sfragment << " ,dfragment "<< dfragment<<endl;
   
    vector<int> sBoundaryNode = boundaryNodeSet[sfragment];
    vector<int> dBoundaryNode = boundaryNodeSet[dfragment];
    for (size_t i = 0; i < sBoundaryNode.size(); ++i) {
        sBN = sBoundaryNode[i];
        for (size_t j = 0; j < dBoundaryNode.size(); ++j){
            dBN = dBoundaryNode[j];
            // cout<< "snode: " << snode << " ,dnode "<< dnode << " ,sBN "<< sBN << " ,dBN "<< dBN  <<endl;
            vector<BoundaryPath> onePairBoundaryPathSet = onePairBoundaryPathOf(snode, dnode, sBN, dBN); 
            boundaryPathSet.insert(boundaryPathSet.end(), onePairBoundaryPathSet.begin(), onePairBoundaryPathSet.end());
        }
    }
    if (sfragment==dfragment){
        vector<Solution> infragmentCostSet = pathRetrievalWithInFragment(snode, dnode, sfragment);
        for (const Solution& solution : infragmentCostSet){
            boundaryPathSet.push_back(BoundaryPath({solution.cost1,solution.cost2,solution.cost1,solution.cost2}, {snode, dnode}));
        }
        
    }
    cout<< "number of boundary paths before dominance check: " << boundaryPathSet.size() <<endl;
    paretoBoundaryPathSet = boundaryPathDominanceCheck(boundaryPathSet);
    cout<< "number of boundary paths after dominance check: " << paretoBoundaryPathSet.size() <<endl;
    return paretoBoundaryPathSet;
}

vector<Solution> B3HEPV::expandPathCostOf(BoundaryPath boundaryPath){
    vector<Solution> pathCostSet;
    for (int i =0; i<boundaryPath.len()-1; i++){
        int currentNode = boundaryPath.getNode(i);
        int currentFragment = fragmentIndex[currentNode-1][0];
        int nextNode =  boundaryPath.getNode(i+1);
        int nextFragment = fragmentIndex[nextNode-1][0];
        if (currentFragment == nextFragment){
            pathCostSet = B3HEPV::combineCostSet(pathCostSet, B3HEPV::pathRetrievalWithInFragment(currentNode, nextNode, currentFragment));
        }
        else{
            int c1 = adjacentLub.at(currentNode).at(nextNode)[0];
            int c2 = adjacentLub.at(currentNode).at(nextNode)[1];
            vector<Solution> betweenFragmentEdge = {Solution(c1, c2)};
            pathCostSet = B3HEPV::combineCostSet(pathCostSet, betweenFragmentEdge);
        }
    }

    return pathCostSet;
}


vector<Solution> B3HEPV::combineCostSet(vector<Solution> costSet1, vector<Solution> costSet2) {
    vector<Solution> result;
    if (!costSet1.size())
        return costSet2;
    if (!costSet2.size())
        return costSet1;
    for (const Solution& solution1 : costSet1) {
        for (const Solution& solution2 : costSet2) {
            Solution combinedSolution = solution1.concatWith(solution2);
            result.push_back(combinedSolution);
        }
    }
    return result;
}






vector<Solution> B3HEPV::pathRetrievalWithInFragment(int snode, int dnode, int fragmentId) {
    numberOfCallOfBoA++;
//     cout<<"snode"<< snode<< " "<< dnode<< " "<<fragmentId <<endl;
    vector<Solution> pathCostSet;
//     string filename = fileFolderName+ "/fragments/fragment" + std::to_string(fragmentId) + ".txt";
//     const char* charFilename = filename.c_str();
    int snodeInFragment = fragmentIndex[snode-1][1];
    int dnodeInFragment = fragmentIndex[dnode-1][1];
    const GraphData currentGraph = graphDataVector[fragmentId+1];
//     cout<< "get current graph" << currentGraph.numOfGnode << " , "<< currentGraph.numOfArcs << endl;
    const GraphData* graphDataPtr = &currentGraph;
//     cout<< "get current graph pointer" << endl;
    // cout<<"snode In fragment"<< snodeInFragment<< " "<< dnodeInFragment<< " "<<fragmentId <<endl;
//     printEdgeVectorsCPP(graphDataPtr);
    unsigned (*solutions)[2] = paretoPathsInFragment(snodeInFragment, dnodeInFragment, graphDataPtr);
    int i = 0;
    while (solutions[i][0] > 0) {
        Solution currentSol(solutions[i][0], solutions[i][1]);
        pathCostSet.push_back(currentSol);
        i++;
    } 
//     cleanupGraphData(&currentGraph);
    return pathCostSet;
}


int B3HEPV::boaPathRetrieval(int snode, int dnode) {
    // convert filename to char *
    int nsolutions = 0;
    const GraphData currentGraph = graphDataVector[0];
//     const char* charFilename = filename.c_str();
    const GraphData* graphDataPtr = &currentGraph;
    
    unsigned (*solutions)[2] = paretoPathsInFragment(snode, dnode, graphDataPtr);
    // Access and print the values
//     cleanupGraphData(&currentGraph);
    int i = 0;
    while (solutions[i][0] > 0) {
//         cout<< "BOA solutions: "<< solutions[i][0] << ", " << solutions[i][1] << endl;
        nsolutions+=1;
        i++;
    }   
    return nsolutions;
}







vector<Solution> B3HEPV::expandPathForBoundaryPathSet(vector<BoundaryPath> boundaryPathSet){
    vector<Solution> expendedPathCostSet;
    for (size_t i = 0; i < boundaryPathSet.size(); ++i) {
        vector<Solution> pathCostWithOneBoundaryPath = B3HEPV::expandPathCostOf(boundaryPathSet[i]);
        expendedPathCostSet.insert(expendedPathCostSet.end(), pathCostWithOneBoundaryPath.begin(), pathCostWithOneBoundaryPath.end());
    }
    return expendedPathCostSet;
}   

// vector<Solution> B3HEPV::dominanceCheck(vector<Solution> superParetoCostSet){
//     vector<Solution> nonDominatedSolutions;
//     Solution currentSol, comparedSol;
//     vector<int> paretoIndex(superParetoCostSet.size(), 1);
//     for (size_t i = 0; i < superParetoCostSet.size(); ++i) {
//         if (paretoIndex[i]==1){
//             currentSol = superParetoCostSet[i];
//             for (size_t j = i+1; j < superParetoCostSet.size(); ++j){
//                 comparedSol = superParetoCostSet[j];
//                 if (currentSol.isDominatedBy(comparedSol)){
//                     paretoIndex[i]=0;
//                 }
//                 if (comparedSol.isDominatedBy(currentSol)){
//                     paretoIndex[j]=0;
//                 }
//             }   
//             if (paretoIndex[i]==1){
//             nonDominatedSolutions.push_back(currentSol);
//             }
//         }
//     }
//     return nonDominatedSolutions;
// }


vector<Solution> B3HEPV::dominanceCheck(vector<Solution> superParetoCostSet){
    vector<Solution> nonDominatedSolutions;
    
    // Sort solutions lexicographically by cost1, then cost2
    std::sort(superParetoCostSet.begin(), superParetoCostSet.end(),
              [](const Solution &a, const Solution &b) {
                  return std::tie(a.cost1, a.cost2) < std::tie(b.cost1, b.cost2);
              });

    int minCost2 = INT_MAX;

    for (const auto& sol : superParetoCostSet) {
        if (sol.cost2 >= minCost2) {
            // This solution is dominated, skip it
            continue;
        }
        minCost2 = sol.cost2;
        nonDominatedSolutions.push_back(sol);
    }

    return nonDominatedSolutions;
}




int B3HEPV::hbor(int snode, int dnode){
    numberOfCallOfBoA = 0;
    vector<BoundaryPath> boundaryPathSet = B3HEPV::paretoBoundaryPathBetween(snode, dnode);
//     for (const auto& path : boundaryPathSet) {
//         for (int i : path.lub) {
//             cout << "lub: " << i << " ";
//         }
//         cout << endl;
//         path.printPath();
//     }
    vector<Solution> superParetoCostSet = B3HEPV::expandPathForBoundaryPathSet(boundaryPathSet);
    cout<< "Number of boundary edges expended: " << numberOfCallOfBoA<<endl;
    vector<Solution> solutions = B3HEPV::dominanceCheck(superParetoCostSet); 
//     for (const Solution& solution : solutions) {
//         std::cout << "HBOR Solutions: (" << solution.cost1 << ", " << solution.cost2 << ")" << std::endl;
//     }
    int nsolutions = solutions.size();
    solutions.clear();
    return nsolutions;
}


void B3HEPV::read_adjacent_table() {
    string filenameEntirGraph = "../Maps/"+ mapName + "-road-d.txt";
    
    GraphData graphData;
    int num_nodes = 0;
    int num_arcs = 0;

    // Initialize graphData
//     initializeGraphDataBOA(&graphData, num_nodes, num_arcs);

    // Read data from file and assign it to graphData
    readDataFromFile(&graphData, filenameEntirGraph);

    graphDataVector.push_back(graphData);

    // Cleanup graphData
//     cleanupGraphData(&graphData);
    
   
    for (int i =1; i<nPartitions+1; i++ ){
        GraphData subGraphData;
        string filenameSubGraph = fileFolderName+"/fragments/fragment"+std::to_string(i-1)+".txt";
//         initializeGraphDataBOA(&subGraphData, num_nodes, num_arcs);
        // Read data from file and assign it to graphData
        readDataFromFile(&subGraphData, filenameSubGraph);
        graphDataVector.push_back(subGraphData);
//         cleanupGraphData(&subGraphData);
    }
}


void B3HEPV::readDataFromFile(GraphData* graphData, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Read num_nodes and num_arcs
    file >> graphData->numOfGnode >> graphData->numOfArcs;

    // Allocate memory for edgeVectors
    graphData->edgeVectors = new int*[graphData->numOfArcs];
    for (int i = 0; i < graphData->numOfArcs; i++) {
        graphData->edgeVectors[i] = new int[4];
    }

    // Read data into edgeVectors
    for (int i = 0; i < graphData->numOfArcs; i++) {
        for (int j = 0; j < 4; j++) {
            file >> graphData->edgeVectors[i][j];
        }
    }
//     printEdgeVectorsCPP(graphData);
    

    file.close();
}

void B3HEPV::cleanupGraphDataCpp(GraphData* graphData) {
    for (int i = 0; i < graphData->numOfArcs; i++) {
        delete[] graphData->edgeVectors[i];
    }
    delete[] graphData->edgeVectors;
}

void B3HEPV::freeGraphDataVector(){
    for (GraphData currentGraph :graphDataVector){
        cleanupGraphDataCpp(&currentGraph);
    }
}


void B3HEPV::cleanupGraphDataVector() {
    for (auto& graphData : graphDataVector) {
        cleanupGraphData(&graphData);
    }
    // Clear the vector
    graphDataVector.clear();
}


// int B3HEPV::boaPathRetrievalFromFile(int snode, int dnode, const string& filename) {
//     // convert filename to char *
//     int nsolutions = 0;
//     const char* charFilename = filename.c_str();
//     unsigned (*solutions)[2] = paretoPathsInFragmentChar(snode, dnode, charFilename);
//     // Access and print the values
//     int i = 0;
//     while (solutions[i][0] > 0) {
//         // cout<< "c1: "<< solutions[i][0] << ", c2: "<< solutions[i][1] << endl;
//         nsolutions+=1;
//         i++;
//     }   
//     return nsolutions;
// }





void precomputation(const string & mapName){
    // tested
    BiobjectiveGraph graph("../Maps/"+mapName+"-road-d.txt");
    try {
        std::string outputFolderName = "../b3hepv/"+mapName;
        auto startTime = std::chrono::high_resolution_clock::now();
        // Read the partition information from a file
        graph.readPartition(outputFolderName+"/kaffpaIndex.txt");
        graph.updateBoundaryNodes();
        std::cout << "Partition Count " << graph.partitionCount << std::endl;
        // Print the boundary nodes
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        std::cout << "Elapsed time of read partition: " << duration.count() << " milliseconds" << std::endl;
        

        startTime = std::chrono::high_resolution_clock::now();
        graph.saveFragments(outputFolderName);
        endTime = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        std::cout << "Elapsed time of fragments saving: " << duration.count() << " milliseconds" << std::endl;
        
        startTime = std::chrono::high_resolution_clock::now();
        graph.saveExistingLUB(outputFolderName);
        endTime = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        std::cout << "Elapsed time of existing LUBS: " << duration.count() << " milliseconds" << std::endl;
                
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    

}


// int main(){
//     testHbor();
//     return 0;
// }

