#include "hborWithB3hepv.hpp"

#include <string>
#include <sstream>
#include <climits>
#include <list>

#undef max

using json = nlohmann::json;
using namespace std;







B3HBORBasic::B3HBORBasic(const std::string& map, int npar) : mapName(map),nPartitions(npar) {
    cout<< "reading" <<endl;
    // read fragment index
    fileFolderName = "../b3hepv/"+mapName;
    
}

void B3HBORBasic::loadEncodedPathView() {
    std::string fileNameFragmentEPV = fileFolderName + "/fragmentEPV.json";
    std::ifstream i_fepv(fileNameFragmentEPV);
    json j_fepv;
    i_fepv >> j_fepv;

    // Clear existing data
    fragmentEncodedPathView.clear();
    int cnt_FragmentPath = 0;
    // Populate boundaryEncodedPathView from json
    for (json::iterator it = j_fepv.begin(); it != j_fepv.end(); ++it) {
        int outerKey = std::stoi(it.key());
        for (json::iterator inner_it = it.value().begin(); inner_it != it.value().end(); ++inner_it) {
            int innerKey = std::stoi(inner_it.key());
            fragmentEncodedPathView[outerKey][innerKey] = inner_it.value().get<std::vector<std::vector<int>>>();
            cnt_FragmentPath++;
        }
    }
    cout<< "Number of encoded fragment paths: " << cnt_FragmentPath << endl;
    
  
    
    
    /* Instead, load bound-based boundary EPV */
    // {snode: {dnoded:[[lb1,lb2,ub1,ub2,path1],[lb1',lb2',ub1',ub2',path2], ...]}}
    // Assuming BoundaryPathCompare is already defined
    auto boundaryPathCompare = [](const BoundaryPath& a, const BoundaryPath& b) {
        if (a.lub[2] == b.lub[2]) {
            return a.lub[3] < b.lub[3];
        }
        return a.lub[2] < b.lub[2];
    };

    
    
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

            for (const auto& entry2 : entry1.value().items()) {
                int dnode = stoi(entry2.key());

                vector<BoundaryPath> pathSet;
                vector<BoundaryPath> reversedPathSet;
                for (const auto& arrayValue : entry2.value()) {
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
                    reversedPathSet.push_back(path.reverse());
                }

                // Sort the pathSet
                std::sort(pathSet.begin(), pathSet.end(), boundaryPathCompare);
                b3boundaryEPV[snode][dnode] = organizeBoundaryPaths(pathSet);
                std::sort(reversedPathSet.begin(), reversedPathSet.end(), boundaryPathCompare);
                b3boundaryEPV[dnode][snode] = organizeBoundaryPaths(reversedPathSet);
//                 cout<< "sdnode" << snode << dnode << endl;
            }
        }
   
   }
    
//     printB3BoundaryEPV();

        
    
 
    cout<< "Number of boundary paths: " << cnt_BoundaryPath << endl;
    int cnt_fragmentPath = 0;

    
    /* load bound-based fragment EPV */
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
            b3fragmentEPV[startNode][endNode] = path;
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


    cout<< "fragments read: " << graphDataVector.size()<<endl;
    
}

void B3HBORBasic::printB3BoundaryEPV() const {
    for (const auto& outerMapEntry : b3boundaryEPV) {
        int snode = outerMapEntry.first;
        std::cout << "snode: " << snode << std::endl;

        for (const auto& innerMapEntry : outerMapEntry.second) {
            int dnode = innerMapEntry.first;
            std::cout << "\tdnode: " << dnode << std::endl;

            int pathSetIndex = 1;
            for (const auto& pathSet : innerMapEntry.second) {
                std::cout << "\t\tPath Set " << pathSetIndex++ << ":" << std::endl;
                
                int pathIndex = 1;
                for (const BoundaryPath& path : pathSet) {
                    std::cout << "\t\t\tPath " << pathIndex++ << ": LUB = [";
                    
                    // Assuming lub is a member of BoundaryPath and is a vector of integers
                    for (size_t i = 0; i < path.lub.size(); ++i) {
                        std::cout << path.lub[i];
                        if (i != path.lub.size() - 1) {
                            std::cout << ", ";
                        }
                    }
                    std::cout << "]";

                    // If you want to print the actual path too
                    std::cout << " Path: [";
                    for (size_t i = 0; i < path.path.size(); ++i) {
                        std::cout << path.path[i];
                        if (i != path.path.size() - 1) {
                            std::cout << ", ";
                        }
                    }
                    std::cout << "]" << std::endl;
                }
            }
        }
    }
}


std::vector<std::vector<BoundaryPath>> B3HBORBasic::organizeBoundaryPaths(std::vector<BoundaryPath>& sortedPathSet) {
    std::vector<std::vector<BoundaryPath>> organizedPaths;
    while (!sortedPathSet.empty()) {
        std::vector<BoundaryPath> currentSet;
        
        // Start with the smallest path
        BoundaryPath currentMinPath = sortedPathSet.front();
        currentSet.push_back(currentMinPath);

        // Check all the other paths for domination by the smallest path
        size_t j = 1;
        while (j < sortedPathSet.size()) {
            if (sortedPathSet[j].isLowerBoundDominatedBy(currentMinPath)) {
                currentSet.push_back(sortedPathSet[j]);
                
                // Erase the dominated path from sortedPathSet and maintain the order
                sortedPathSet.erase(sortedPathSet.begin() + j);

                // Do not increment j, because after erasing, the next element has now shifted to position j
            } else {
                j++;  // Move to the next path only if the current path is not dominated
            }
        }

        // Remove the current minimum path from sortedPathSet
        sortedPathSet.erase(sortedPathSet.begin());

        organizedPaths.push_back(currentSet);
    }

    return organizedPaths;
}


void B3HBORBasic::loadBoundaryNodes(){
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
}





void B3HBORBasic::loadFragments() {
  
    GraphData graphData;
    int num_nodes = 0;
    int num_arcs = 0;

    // Initialize graphData
//     initializeGraphData(&graphData, num_nodes, num_arcs);
   
    for (int i =1; i<nPartitions+1; i++ ){
        string filenameSubGraph = fileFolderName+"/fragments/fragment"+std::to_string(i-1)+".txt";
        // initializeGraphData(&graphData, num_nodes, num_arcs);
        // Read data from file and assign it to graphData
        readGraphDataFromFile(&graphData, filenameSubGraph);
        graphDataVector.push_back(graphData);
        // cleanupGraphData(&graphData);
    }
}
void B3HBORBasic::readGraphDataFromFile(GraphData* graphData, const std::string& filename) {
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



void B3HBORBasic::readOriginGraphDataFromFile(GraphData* graphData) {
    string filename = "../Maps/"+mapName+"-road-d.txt";
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Read num_nodes and num_arcs
    file >> graphData->numOfGnode >> graphData->numOfArcs;
    
    numVertices = graphData->numOfGnode;
    adjacencyList.resize(numVertices);
    partitions.resize(numVertices);


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
    file.close();
}



void B3HBORBasic::readOriginGraph(){
    GraphData graphData;
    int num_nodes = 0;
    int num_arcs = 0;
    // Initialize graphData
//     initializeGraphData(&graphData, num_nodes, num_arcs);
    // Read data from file and assign it to graphData
    readOriginGraphDataFromFile(&graphData);
    graphDataVector.push_back(graphData);
}






vector<BiobjectivePath> B3HBORBasic::boaPathRetrievalWithInFragment(int snode, int dnode, int fragmentId) {
    vector<BiobjectivePath> pathCostSet;
    int snodeInFragment = snode;
    int dnodeInFragment = dnode;
    if (fragmentId!=-1){ //using fragment graphs
        snodeInFragment = fragmentIndex[snode-1][1];
        dnodeInFragment = fragmentIndex[dnode-1][1];
    }
    // cout<< "snodeInFragment: "<<snodeInFragment<<dnodeInFragment<<endl;
    const GraphData currentGraph = graphDataVector[fragmentId+1];
    const GraphData* graphDataPtr = &currentGraph;
    unsigned (*solutions)[2] = paretoPathsInFragment(snodeInFragment, dnodeInFragment, graphDataPtr);
    int i = 0;
    while (solutions[i][0] > 0) {
        BiobjectivePath currentSol(solutions[i][0], solutions[i][1]);
        pathCostSet.push_back(currentSol);
        i++;
    } 
//     cout<<pathCostSet.size()<<endl;
    return pathCostSet;
}

// vector<BiobjectivePath> B3HBORBasic::namorPathRetrievalWithInFragment(int snode, int dnode, int fragmentId) {
//     vector<BiobjectivePath> pathCostSet;
//     int snodeInFragment = snode;
//     int dnodeInFragment = dnode;
//     if (fragmentId!=-1){ //using fragment graphs
//         snodeInFragment = fragmentIndex[snode-1][1];
//         dnodeInFragment = fragmentIndex[dnode-1][1];
//     }
//     // cout<< "snodeInFragment: "<<snodeInFragment<<dnodeInFragment<<endl;
//     const GraphData currentGraph = graphDataVector[fragmentId+1];
//     const GraphData* graphDataPtr = &currentGraph;
//     unsigned (*solutions)[2] = namorInFragment(snodeInFragment, dnodeInFragment, graphDataPtr);
//     int i = 0;
//     while (solutions[i][0] > 0) {
//         BiobjectivePath currentSol(solutions[i][0], solutions[i][1]);
//         pathCostSet.push_back(currentSol);
//         i++;
//     } 
//     cout<<pathCostSet.size()<<endl;
//     return pathCostSet;
// }




void B3HBORBasic::loadFragmentIndex(){
    // ori fragment index
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
        std::vector<int> nodeIndexVec(nodeIndex.begin(), nodeIndex.end());
        fragmentIndex.push_back(nodeIndexVec);
    }
    fin.close();
}


std::vector<BiobjectivePath> B3HBORBasic::generateCombinations(
    const std::vector<BiobjectivePath>& vec1,
    const std::vector<BiobjectivePath>& vec2,
    const std::vector<BiobjectivePath>& vec3
) {
    std::vector<BiobjectivePath> result;

    for (const auto& path1 : vec1) {
        for (const auto& path2 : vec2) {
            for (const auto& path3 : vec3) {
                // Concatenate the three paths
                BiobjectivePath newPath = path1.concatWith(path2).concatWith(path3);
                // Add the new path to the result vector
                result.push_back(newPath);
//                 cout<< "newPath: " << newPath.cost1 << newPath.cost2 << endl;
            }
        }
    }
    
    return result;
}


vector<BiobjectivePath> B3HBORBasic::dominanceCheck(vector<BiobjectivePath> superParetoCostSet){
    vector<BiobjectivePath> nonDominatedSolutions;

    // Sort solutions lexicographically by cost1, then cost2
    std::sort(superParetoCostSet.begin(), superParetoCostSet.end(),
              [](const BiobjectivePath &a, const BiobjectivePath &b) {
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

std::vector<BoundaryPath> B3HBORBasic::reversePaths(const std::vector<BoundaryPath>& paths) {
    std::vector<BoundaryPath> reversed_paths;
    reversed_paths.reserve(paths.size()); // Reserve memory for efficiency

    for (const BoundaryPath& path : paths) {
        BoundaryPath reversed_path = path.reverse();
        reversed_paths.push_back(reversed_path);
    }

    return reversed_paths;
}

// vector<BoundaryPath> B3HBORBasic::boundaryPathDominanceCheck(vector<BoundaryPath> boundaryPathSet){
//     // for each boundaryPath, the first four elements are (LB1, LB2, UB1, UB2), respectively
//     vector<BoundaryPath> paretoBoundaryPath;
//     BoundaryPath currentPath, comparedPath;
//     vector<int> paretoIndex(boundaryPathSet.size(), 1);
//     for (size_t i = 0; i < boundaryPathSet.size(); ++i) {
//         if (paretoIndex[i]==1){
//             currentPath = boundaryPathSet[i];
//             for (size_t j = i+1; j < boundaryPathSet.size(); ++j){
//                 comparedPath = boundaryPathSet[j];
//                 if (currentPath.isDominatedBy(comparedPath)){
//                     paretoIndex[i]=0;
//                 }
//                 else if (comparedPath.isDominatedBy(currentPath)){
//                     paretoIndex[j]=0;
//                 }
//             }   
//         }
//         if (paretoIndex[i]==1){
//             paretoBoundaryPath.push_back(currentPath);
//         }
//     }
//     return paretoBoundaryPath;
// }

vector<BoundaryPath> B3HBORBasic::boundaryPathDominanceCheck(vector<BoundaryPath> boundaryPathSet){
    // Sort the boundary paths by the first and second elements of lub.
    std::sort(boundaryPathSet.begin(), boundaryPathSet.end(), [](const BoundaryPath& a, const BoundaryPath& b) {
        if (a.lub[0] == b.lub[0]) {
            return a.lub[1] < b.lub[1];
        }
        return a.lub[0] < b.lub[0];
    });
    
    // Using a list to maintain the order of insertion and allow for efficient removals.
    std::list<BoundaryPath> survivedPaths(boundaryPathSet.begin(), boundaryPathSet.end());
    
    for (auto it_i = survivedPaths.begin(); it_i != survivedPaths.end(); ++it_i) {
        auto it_j = it_i;
        advance(it_j, 1);
        while (it_j != survivedPaths.end()) {
            if (it_j->isDominatedBy(*it_i)) {
                // If j is dominated by i, remove j from the list.
                it_j = survivedPaths.erase(it_j);
            } else {
                ++it_j;
            }
        }   
    }
    
    return vector<BoundaryPath>(survivedPaths.begin(), survivedPaths.end());
}








vector<BiobjectivePath> B3HBORBasic::pathRetrievalWithInFragment(int snode, int dnode) {
    numberOfExpendedEdges++;
    vector<BiobjectivePath> onePairBoundaryPathSet;

    if (snode == dnode) {
        onePairBoundaryPathSet.push_back(BiobjectivePath(0,0));
    } else {
        auto snodeIt = fragmentEncodedPathView.find(snode);
        auto dnodeIt = fragmentEncodedPathView.find(dnode);

        if (snodeIt != fragmentEncodedPathView.end() && snodeIt->second.count(dnode) > 0) {
            for (const auto& biCost : snodeIt->second.at(dnode)) {
                BiobjectivePath headPath = BiobjectivePath(biCost[0], biCost[1]);
                onePairBoundaryPathSet.push_back(headPath);
            }
        } else if (dnodeIt != fragmentEncodedPathView.end() && dnodeIt->second.count(snode) > 0) {
            for (const auto& biCost : dnodeIt->second.at(snode)) {
                BiobjectivePath headPath = BiobjectivePath(biCost[0], biCost[1]);
                onePairBoundaryPathSet.push_back(headPath);
            }
        } else {
            throw std::runtime_error("Both nodes should be in the same fragment and at least one node should be a boundary node!");
        }
    }

    return onePairBoundaryPathSet;
}

vector<BiobjectivePath> B3HBORBasic::combineCostSet(vector<BiobjectivePath> costSet1, vector<BiobjectivePath> costSet2) {
    vector<BiobjectivePath> result;
    if (!costSet1.size())
        return costSet2;
    if (!costSet2.size())
        return costSet1;
    for (const BiobjectivePath& solution1 : costSet1) {
        for (const BiobjectivePath& solution2 : costSet2) {
            BiobjectivePath combinedSolution = solution1.concatWith(solution2);
            result.push_back(combinedSolution);
        }
    }
    return result;
}



vector<BiobjectivePath> B3HBORBasic::expandPathCostOf(BoundaryPath boundaryPath){
    vector<BiobjectivePath> pathCostSet;
    for (int i =0; i<boundaryPath.len()-1; i++){
        int currentNode = boundaryPath.getNode(i);
        int currentFragment = fragmentIndex[currentNode-1][0];
        int nextNode =  boundaryPath.getNode(i+1);
        int nextFragment = fragmentIndex[nextNode-1][0];
        if (currentFragment == nextFragment){
            pathCostSet = combineCostSet(pathCostSet, pathRetrievalWithInFragment(currentNode, nextNode));
        }
        else{
            int c1 = adjacentLub.at(currentNode).at(nextNode)[0];
            int c2 = adjacentLub.at(currentNode).at(nextNode)[1];
            vector<BiobjectivePath> betweenFragmentEdge = {BiobjectivePath(c1, c2)};
            pathCostSet = combineCostSet(pathCostSet, betweenFragmentEdge);
        }
    }

    return pathCostSet;
}




vector<BiobjectivePath> B3HBORBasic::expandPathForBoundaryPathSet(vector<BoundaryPath> boundBasedBoundaryPathSet) {
    vector<BiobjectivePath> expendedPathCostSet;
    for (size_t i = 0; i < boundBasedBoundaryPathSet.size(); ++i) {
        vector<BiobjectivePath> pathCostWithOneBoundaryPath = expandPathCostOf(boundBasedBoundaryPathSet[i]);
        expendedPathCostSet.insert(expendedPathCostSet.end(), pathCostWithOneBoundaryPath.begin(), pathCostWithOneBoundaryPath.end());
    }
    return expendedPathCostSet;
}

void B3HBORBasic::populateSortedBoundaryPairs() {
    auto boundaryPathCompare = [](const BoundaryPath& a, const BoundaryPath& b) {
        if (a.lub[0] == b.lub[0]) {
            return a.lub[1] < b.lub[1];
        }
        return a.lub[0] < b.lub[0];
    };

    for (int sFragment = 0; sFragment < boundaryNodeSet.size(); ++sFragment) {
        for (int dFragment = 0; dFragment < boundaryNodeSet.size(); ++dFragment) {
            vector<int> sBoundaryNode = boundaryNodeSet[sFragment];
            vector<int> dBoundaryNode = boundaryNodeSet[dFragment];
            
            vector<pair<int, int>> sortedBoundaryPairs;
            for (const int& sBN : sBoundaryNode) {
                for (const int& dBN : dBoundaryNode) {
                    if (b3boundaryEPV[sBN].find(dBN) != b3boundaryEPV[sBN].end()) {
                        sortedBoundaryPairs.emplace_back(sBN, dBN);
                    }
                }
            }
            std::sort(sortedBoundaryPairs.begin(), sortedBoundaryPairs.end(),
                [&](const pair<int, int>& pair1, const pair<int, int>& pair2) {
                    const BoundaryPath& path1 = b3boundaryEPV[pair1.first][pair1.second][0][0];
                    const BoundaryPath& path2 = b3boundaryEPV[pair2.first][pair2.second][0][0];
                    return boundaryPathCompare(path1, path2);
                }
            );

            sortedBoundaryPairsMap[sFragment][dFragment] = sortedBoundaryPairs;
        }
    }
}



void B3HBORBasic::load(){
    readOriginGraph();
    loadBoundaryNodes();
    loadEncodedPathView();
    loadFragmentIndex();
    loadFragments();
    populateSortedBoundaryPairs();
    cout<< "loaded!" << endl;
}



std::vector<BoundaryPath> B3HBORBasic::onePairB3PathOf(int snode, int dnode, int sBN, int dBN, HeapManager& heapManager) {
    std::vector<BoundaryPath> onePairBoundaryPathSet;
    BoundaryPath headPath, tailPath, onePairPath;

    // Get headPath
    auto head_it = b3fragmentEPV.find(snode);
    if (snode == sBN || (head_it != b3fragmentEPV.end() && head_it->second.find(sBN) != head_it->second.end())) {
        headPath = (snode == sBN) ? BoundaryPath({0, 0, 0, 0}, {snode}) : head_it->second[sBN];
    } else {
        headPath = b3fragmentEPV[sBN][snode].reverse();
    }

    // Similar approach for tailPath
    auto tail_it = b3fragmentEPV.find(dBN);
    if (dnode == dBN || (tail_it != b3fragmentEPV.end() && tail_it->second.find(dnode) != tail_it->second.end())) {
        tailPath = (dnode == dBN) ? BoundaryPath({0, 0, 0, 0}, {dnode}) : tail_it->second[dnode];
    } else {
        tailPath = b3fragmentEPV[dnode][dBN].reverse();
    }

    // Processing boundary paths
    auto sBN_it = b3boundaryEPV.find(sBN);
    if (sBN == dBN) {
        onePairPath = headPath.concatWith(tailPath);
        onePairBoundaryPathSet.push_back(onePairPath);
    } else if (sBN_it != b3boundaryEPV.end() && sBN_it->second.find(dBN) != sBN_it->second.end()) {
        const std::vector<std::vector<BoundaryPath>>& nestedPathSets = sBN_it->second[dBN];
        
        for (const auto& pathSet : nestedPathSets) {
            for (const auto& interPath : pathSet) {
                onePairPath = headPath.concatWith(interPath).concatWith(tailPath);
                
                bool isDominated = false;
                for (int comp = 1; comp <= 4; ++comp) {
                    if (heapManager.isDominatedBySmallest(onePairPath, comp)) {
                        isDominated = true;
                        break;
                    }
                }

                if (!isDominated) {
                    onePairBoundaryPathSet.push_back(onePairPath);
                    for (int comp = 1; comp <= 4; ++comp) {
                        heapManager.pushToHeap(onePairPath, comp);
                    }
                } else {
                    break;
                }
            }
        }
    }

    return onePairBoundaryPathSet;
}



vector<BoundaryPath> B3HBORBasic::paretoBoundaryPathBetween(int snode, int dnode) {
    vector<BoundaryPath> boundaryPathSet;
    // Define a min-heap for boundary paths
//     std::priority_queue<BoundaryPath, vector<BoundaryPath>, BoundaryPathCompare> minHeap;
//     std::priority_queue<BoundaryPath, vector<BoundaryPath>, BoundaryPathCompareSencondComp> minHeapSecondComp;
    // Instantiate the HeapManager
    HeapManager heapManager;
    
    int sfragment = fragmentIndex[snode-1][0];
    int dfragment = fragmentIndex[dnode-1][0];
    

    // Sorting the boundary node pairs based on the shortest paths between them
//     const vector<pair<int, int>>& sortedBoundaryPairs = sortedBoundaryPairsMap[sfragment][dfragment];

//     for (const auto& pair : sortedBoundaryPairs) {
//         int sBN = pair.first;
//         int dBN = pair.second;
//         vector<BoundaryPath> onePairBoundaryPathSet = onePairB3PathOf(snode, dnode, sBN, dBN, heapManager);
//         boundaryPathSet.insert(boundaryPathSet.end(), onePairBoundaryPathSet.begin(), onePairBoundaryPathSet.end());
//     }
    
    
    vector<int> sBoundaryNode = boundaryNodeSet[sfragment];
    vector<int> dBoundaryNode = boundaryNodeSet[dfragment];
    
    for (size_t i = 0; i < sBoundaryNode.size(); ++i) {
        
        int sBN = sBoundaryNode[i];
        for (size_t j = 0; j < dBoundaryNode.size(); ++j){
            int dBN = dBoundaryNode[j];
            vector<BoundaryPath> onePairBoundaryPathSet = onePairB3PathOf(snode, dnode, sBN, dBN, heapManager);
            boundaryPathSet.insert(boundaryPathSet.end(), onePairBoundaryPathSet.begin(), onePairBoundaryPathSet.end());

        }
    }
    

    cout << "number of boundary paths before dominance check: " << boundaryPathSet.size() << endl;
//     for (const BoundaryPath& bp : boundaryPathSet) {
//         bp.printPath();  // Assuming BoundaryPath has a print method
//         bp.printLub();  // Assuming BoundaryPath has a print method
//         std::cout << "-------------------" << std::endl;  // Separator for clarity
//     }
    boundaryPathSet = boundaryPathDominanceCheck(boundaryPathSet);
    cout << "number of boundary paths after dominance check: " << boundaryPathSet.size() << endl;
//     for (const BoundaryPath& bp : boundaryPathSet) {
//         bp.printPath();  // Assuming BoundaryPath has a print method
//         bp.printLub();  // Assuming BoundaryPath has a print method
//         std::cout << "-------------------" << std::endl;  // Separator for clarity
//     }

    return boundaryPathSet;
}




int B3HBORBasic::hbor(int snode, int dnode){
    numberOfExpendedEdges = 0;
    vector<BiobjectivePath> solutionSet;
    int sBN, dBN;
  
    int sfragment = fragmentIndex[snode-1][0];
    int dfragment = fragmentIndex[dnode-1][0];
    
    cout<< "sfragment: " << sfragment << " ,dfragment "<< dfragment<<endl;
    
    vector<int> sBoundaryNode = boundaryNodeSet[sfragment];
    vector<int> dBoundaryNode = boundaryNodeSet[dfragment];
    
    if (sfragment==dfragment){
        vector<BiobjectivePath> infragmentCostSet = boaPathRetrievalWithInFragment(snode, dnode, sfragment);
        solutionSet.insert(solutionSet.end(), infragmentCostSet.begin(), infragmentCostSet.end());
    }
    
   
    vector<BoundaryPath> boundaryPathSet = paretoBoundaryPathBetween(snode, dnode);
    
  
    vector<BiobjectivePath> superParetoCostSet = expandPathForBoundaryPathSet(boundaryPathSet);
    
    
    solutionSet.insert(solutionSet.end(), superParetoCostSet.begin(), superParetoCostSet.end());
      
    vector<BiobjectivePath> solutions = dominanceCheck(solutionSet); 
    int nsolutions = solutions.size();
    solutions.clear();
    cout<< "Number of boundary edges expended: " << numberOfExpendedEdges<<endl;
    return nsolutions;
    
    
    
}

int B3HBORBasic::boa(int snode, int dnode){
    vector<BiobjectivePath> solutionSet = boaPathRetrievalWithInFragment(snode, dnode, -1);
//     for (size_t j = 0; j < solutionSet.size(); ++j){
//         cout<< "result: " << solutionSet[j].cost1 <<", " <<solutionSet[j].cost2 <<endl;
//     }

    return solutionSet.size();
    
}




void B3HBORBasic::cleanupGraphDataCpp(GraphData* graphData) {
    for (int i = 0; i < graphData->numOfArcs; i++) {
        delete[] graphData->edgeVectors[i];
    }
    delete[] graphData->edgeVectors;
}

void B3HBORBasic::freeGraphDataVector(){
    for (GraphData currentGraph :graphDataVector){
        cleanupGraphDataCpp(&currentGraph);
    }
}




