#include "hborWithB3hepv.hpp"

#include <string>
#include <sstream>
#include <climits>

#undef max

using json = nlohmann::json;
using namespace std;


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
    
    /* no boundary EPV */
//     std::string fileNameBoundaryEPV = fileFolderName + "/boundaryEPV.json";
//     std::ifstream i_bepv(fileNameBoundaryEPV);
//     json j_bepv;
//     i_bepv >> j_bepv;

//     // Clear existing data
//     boundaryEncodedPathView.clear();
    
//     int cnt_BoundaryPath = 0;
//     // Populate boundaryEncodedPathView from json
//     for (json::iterator it = j_bepv.begin(); it != j_bepv.end(); ++it) {
//         int outerKey = std::stoi(it.key());
//         for (json::iterator inner_it = it.value().begin(); inner_it != it.value().end(); ++inner_it) {
//             cnt_BoundaryPath ++;
//             int innerKey = std::stoi(inner_it.key());
//             boundaryEncodedPathView[outerKey][innerKey] = inner_it.value().get<std::vector<std::vector<int>>>();
//         }
//     }
//     cout<< "Number of encoded bundary paths: " << cnt_BoundaryPath << endl;
    
    /* Instead, load bound-based boundary EPV */
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
                        b3boundaryEPV[snode][dnode] = pathSet;
                    }
                }
            }
        }
    }
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
    cout<<pathCostSet.size()<<endl;
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

vector<BoundaryPath> B3HBORBasic::boundaryPathDominanceCheck(vector<BoundaryPath> boundaryPathSet){
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

vector<BoundaryPath> B3HBORBasic::onePairB3PathOf(int snode, int dnode, int sBN, int dBN) {
    // cout << snode << dnode<<sBN << dBN<<endl;
    vector<BoundaryPath> onePairBoundaryPathSet;
    BoundaryPath headPath, tailPath, onePairPath;

    if (snode == sBN) {
        headPath = BoundaryPath({0, 0, 0, 0}, {snode});
    } else {
        if (b3fragmentEPV.count(snode) > 0 && b3fragmentEPV.at(snode).count(sBN) > 0) {
            headPath = b3fragmentEPV.at(snode).at(sBN);
        } else if (b3fragmentEPV.count(sBN) > 0 && b3fragmentEPV.at(sBN).count(snode) > 0) {
            headPath = b3fragmentEPV.at(sBN).at(snode).reverse();
        } else {
            throw std::runtime_error("Missing entry in b3fragmentEPV");
        }
    }

    if (dnode == dBN) {
        tailPath = BoundaryPath({0, 0, 0, 0}, {dnode});
    } else {
        if (b3fragmentEPV.count(dBN) > 0 && b3fragmentEPV.at(dBN).count(dnode) > 0) {
            tailPath = b3fragmentEPV.at(dBN).at(dnode);
        } else if (b3fragmentEPV.count(dnode) > 0 && b3fragmentEPV.at(dnode).count(dBN) > 0) {
            tailPath = b3fragmentEPV.at(dnode).at(dBN).reverse();
        } else {
            throw std::runtime_error("Missing entry in b3fragmentEPV");
        }
    }
    
    if (sBN == dBN){
        onePairPath = headPath.concatWith(tailPath);
        onePairBoundaryPathSet.push_back(onePairPath);
        return onePairBoundaryPathSet;

    }
    else if (b3boundaryEPV.count(sBN) > 0 && b3boundaryEPV.at(sBN).count(dBN) > 0) {
        const vector<BoundaryPath>& interPathSetBetweenSbnAndDbn = b3boundaryEPV.at(sBN).at(dBN);
        for (const auto& interPath : interPathSetBetweenSbnAndDbn) {
            onePairPath = headPath.concatWith(interPath).concatWith(tailPath);
            onePairBoundaryPathSet.push_back(onePairPath);
        }
    }
    else if (b3boundaryEPV.count(dBN) > 0 && b3boundaryEPV.at(dBN).count(sBN) > 0){
        const vector<BoundaryPath>& interPathSetBetweenSbnAndDbn = reversePaths(b3boundaryEPV.at(dBN).at(sBN));
        for (const auto& interPath : interPathSetBetweenSbnAndDbn) {
            onePairPath = headPath.concatWith(interPath).concatWith(tailPath);
            onePairBoundaryPathSet.push_back(onePairPath);
        }
    }
    // cout<<onePairBoundaryPathSet.size()<<endl;
    return boundaryPathDominanceCheck(onePairBoundaryPathSet);
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




vector<BiobjectivePath> B3HBORBasic::onePairBoundaryPathOf(int snode, int dnode, int sBN, int dBN) {
    vector<BoundaryPath> boundBasedBoundaryPathSet = onePairB3PathOf(snode, dnode, sBN, dBN);
    vector<BiobjectivePath> expendedPathCostSet;
    for (size_t i = 0; i < boundBasedBoundaryPathSet.size(); ++i) {
        vector<BiobjectivePath> pathCostWithOneBoundaryPath = expandPathCostOf(boundBasedBoundaryPathSet[i]);
        expendedPathCostSet.insert(expendedPathCostSet.end(), pathCostWithOneBoundaryPath.begin(), pathCostWithOneBoundaryPath.end());
    }
    return dominanceCheck(expendedPathCostSet);
}





void B3HBORBasic::load(){
    readOriginGraph();
    loadBoundaryNodes();
    loadEncodedPathView();
    loadFragmentIndex();
    loadFragments();
    cout<< "loaded!" << endl;
}




int B3HBORBasic::hbor(int snode, int dnode){
    numberOfExpendedEdges =0;
    cout<< "snodednode" << snode << dnode <<endl;
    vector<BiobjectivePath> solutionSet;
    int sBN, dBN;

    cout<< fragmentIndex.size() <<endl;
    
    int sfragment = fragmentIndex[snode-1][0];
    int dfragment = fragmentIndex[dnode-1][0];
    
    cout<< "sfragment: " << sfragment << " ,dfragment "<< dfragment<<endl;
    
    vector<int> sBoundaryNode = boundaryNodeSet[sfragment];
    vector<int> dBoundaryNode = boundaryNodeSet[dfragment];
    
    if (sfragment==dfragment){
        vector<BiobjectivePath> infragmentCostSet = boaPathRetrievalWithInFragment(snode, dnode, sfragment);
        solutionSet.insert(solutionSet.end(), infragmentCostSet.begin(), infragmentCostSet.end());
    }
    
    for (size_t i = 0; i < sBoundaryNode.size(); ++i) {
        
        sBN = sBoundaryNode[i];
        for (size_t j = 0; j < dBoundaryNode.size(); ++j){
            dBN = dBoundaryNode[j];
//             cout<< "sBN" << sBN << dBN <<endl;
            vector<BiobjectivePath> onePairBoundaryPathSet = onePairBoundaryPathOf(snode, dnode, sBN, dBN); 
            solutionSet.insert(solutionSet.end(), onePairBoundaryPathSet.begin(), onePairBoundaryPathSet.end());
            
        }
    }

    cout<< "Number of boundary edges expended: " << numberOfExpendedEdges<<endl;
    cout<< "boundary path size: " <<solutionSet.size() <<endl;
    
    vector<BiobjectivePath> paretoSet = dominanceCheck(solutionSet);
//     for (size_t j = 0; j < paretoSet.size(); ++j){
//         cout<< "result: " << paretoSet[j].cost1 <<", " <<paretoSet[j].cost2 <<endl;
//     }

    return paretoSet.size();
    
}

int B3HBORBasic::boa(int snode, int dnode){
    cout<< "snodednode" << snode << dnode <<endl;
    vector<BiobjectivePath> solutionSet = boaPathRetrievalWithInFragment(snode, dnode, -1);
//     for (size_t j = 0; j < solutionSet.size(); ++j){
//         cout<< "result: " << solutionSet[j].cost1 <<", " <<solutionSet[j].cost2 <<endl;
//     }

    return solutionSet.size();
    
}

// int B3HBORBasic::namor(int snode, int dnode){
//     cout<< "snodednode" << snode << dnode <<endl;
//     vector<BiobjectivePath> solutionSet = namorPathRetrievalWithInFragment(snode, dnode, -1);
// //     for (size_t j = 0; j < solutionSet.size(); ++j){
// //         cout<< "result: " << solutionSet[j].cost1 <<", " <<solutionSet[j].cost2 <<endl;
// //     }

//     return solutionSet.size();
    
// }


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