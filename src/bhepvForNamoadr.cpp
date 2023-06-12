#include "bhepvForNamoadr.hpp"

#include <string>
#include <sstream>
#include <climits>

using json = nlohmann::json;
using namespace std;

HBORBasic::HBORBasic(const std::string& map, int npar) : mapName(map),nPartitions(npar) {
    cout<< "reading" <<endl;
    // read fragment index
    fileFolderName = "../bhepv/"+mapName;
    
}

void HBORBasic::loadEncodedPathView() {
    std::string fileNameFragmentEPV = fileFolderName + "/fragmentEPV.json";
    std::ifstream i_fepv(fileNameFragmentEPV);
    json j_fepv;
    i_fepv >> j_fepv;

    // Clear existing data
    fragmentEncodedPathView.clear();

    // Populate boundaryEncodedPathView from json
    for (json::iterator it = j_fepv.begin(); it != j_fepv.end(); ++it) {
        int outerKey = std::stoi(it.key());
        for (json::iterator inner_it = it.value().begin(); inner_it != it.value().end(); ++inner_it) {
            int innerKey = std::stoi(inner_it.key());
            fragmentEncodedPathView[outerKey][innerKey] = inner_it.value().get<std::vector<std::vector<int>>>();
        }
    }
    
    std::string fileNameBoundaryEPV = fileFolderName + "/boundaryEPV.json";
    std::ifstream i_bepv(fileNameBoundaryEPV);
    json j_bepv;
    i_bepv >> j_bepv;

    // Clear existing data
    boundaryEncodedPathView.clear();

    // Populate boundaryEncodedPathView from json
    for (json::iterator it = j_bepv.begin(); it != j_bepv.end(); ++it) {
        int outerKey = std::stoi(it.key());
        for (json::iterator inner_it = it.value().begin(); inner_it != it.value().end(); ++inner_it) {
            int innerKey = std::stoi(inner_it.key());
            boundaryEncodedPathView[outerKey][innerKey] = inner_it.value().get<std::vector<std::vector<int>>>();
        }
    }
    
    
    
}

void HBORBasic::loadBoundaryNodes(){
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


void HBORBasic::loadFragments() {
  
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
void HBORBasic::readGraphDataFromFile(GraphData* graphData, const std::string& filename) {
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



void HBORBasic::readOriginGraphDataFromFile(GraphData* graphData) {
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



void HBORBasic::readOriginGraph(){
    GraphData graphData;
    int num_nodes = 0;
    int num_arcs = 0;
    // Initialize graphData
//     initializeGraphData(&graphData, num_nodes, num_arcs);
    // Read data from file and assign it to graphData
    readOriginGraphDataFromFile(&graphData);
    graphDataVector.push_back(graphData);
}






// vector<BiobjectivePath> HBORBasic::boaPathRetrievalWithInFragment(int snode, int dnode, int fragmentId) {
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
//     unsigned (*solutions)[2] = paretoPathsInFragment(snodeInFragment, dnodeInFragment, graphDataPtr);
//     int i = 0;
//     while (solutions[i][0] > 0) {
//         BiobjectivePath currentSol(solutions[i][0], solutions[i][1]);
//         pathCostSet.push_back(currentSol);
//         i++;
//     } 
//     cout<<pathCostSet.size()<<endl;
//     return pathCostSet;
// }

vector<BiobjectivePath> HBORBasic::namorPathRetrievalWithInFragment(int snode, int dnode, int fragmentId) {
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
    unsigned (*solutions)[2] = namorInFragment(snodeInFragment, dnodeInFragment, graphDataPtr);
    int i = 0;
    while (solutions[i][0] > 0) {
        BiobjectivePath currentSol(solutions[i][0], solutions[i][1]);
        pathCostSet.push_back(currentSol);
        i++;
    } 
    cout<<pathCostSet.size()<<endl;
    return pathCostSet;
}




void HBORBasic::loadFragmentIndex(){
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


std::vector<BiobjectivePath> HBORBasic::generateCombinations(
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

// vector<BiobjectivePath> HBORBasic::dominanceCheck(vector<BiobjectivePath> superParetoCostSet){
// //     cout<< "superParetoCostSet size: " << superParetoCostSet.size() << endl;
//     vector<BiobjectivePath> nonDominatedSolutions;
//     BiobjectivePath currentSol, comparedSol;
//     vector<int> paretoIndex(superParetoCostSet.size(), 1);
//     for (size_t i = 0; i < superParetoCostSet.size(); ++i) {
//         if (paretoIndex[i]==1){
//             currentSol = superParetoCostSet[i];
//             for (size_t j = i+1; j < superParetoCostSet.size(); ++j){
//                 comparedSol = superParetoCostSet[j];
//                 if (currentSol.isDominatedBy(comparedSol)){
//                     paretoIndex[i]=0;
//                 }
//                 if (comparedSol.isDominatedBy(currentSol)|| currentSol.eq(comparedSol)){ // remove duplication
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

vector<BiobjectivePath> HBORBasic::dominanceCheck(vector<BiobjectivePath> superParetoCostSet){
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



vector<BiobjectivePath> HBORBasic::onePairBoundaryPathOf(int snode, int dnode, int sBN, int dBN) {
    // cout << snode << dnode<<sBN << dBN<<endl;
    vector<BiobjectivePath> onePairBoundaryPathSet;
    vector<BiobjectivePath> headPathSet, tailPathSet, interPathSet;

    if (snode == sBN) {
        headPathSet.push_back(BiobjectivePath(0,0));
    } else {
        if (fragmentEncodedPathView.count(sBN) > 0 && fragmentEncodedPathView.at(sBN).count(snode) > 0) {
            for (const auto& biCost : fragmentEncodedPathView.at(sBN).at(snode)) {
                BiobjectivePath headPath = BiobjectivePath(biCost[0],biCost[1]);
                headPathSet.push_back(headPath);
            }
        } else {
            throw std::runtime_error("Missing entry in fragmentEncodedPathView");
        }
    }
    
    if (dnode == dBN) {
        tailPathSet.push_back(BiobjectivePath(0,0));
    } else {
        if (fragmentEncodedPathView.count(dBN) > 0 && fragmentEncodedPathView.at(dBN).count(dnode) > 0) {
            for (const auto& biCost : fragmentEncodedPathView.at(dBN).at(dnode)) {
                BiobjectivePath tailPath = BiobjectivePath(biCost[0],biCost[1]);
                tailPathSet.push_back(tailPath);
            }
        } else {
            throw std::runtime_error("Missing entry in fragmentEncodedPathView");
        }
    }    
    
    if (sBN == dBN){
        interPathSet.push_back(BiobjectivePath(0,0));
    }
    else if (boundaryEncodedPathView.count(sBN) > 0 && boundaryEncodedPathView.at(sBN).count(dBN) > 0) {
        for (const auto& biCost : boundaryEncodedPathView.at(sBN).at(dBN)) {
            BiobjectivePath interPath = BiobjectivePath(biCost[0],biCost[1]);
            interPathSet.push_back(interPath);
        }
    } else {
        throw std::runtime_error("Missing entry in boundaryEncodedPathView");
    }
    // cout<<onePairBoundaryPathSet.size()<<endl;
    return generateCombinations(headPathSet, interPathSet, tailPathSet);
//     return dominanceCheck(generateCombinations(headPathSet, interPathSet, tailPathSet));
}









void HBORBasic::load(){
    readOriginGraph();
    loadBoundaryNodes();
    loadEncodedPathView();
    loadFragmentIndex();
    loadFragments();
    cout<< "loaded!" << endl;
}




// int HBORBasic::hbor(int snode, int dnode){
//     cout<< "snodednode" << snode << dnode <<endl;
//     vector<BiobjectivePath> solutionSet;
//     int sBN, dBN;
    
//     int sfragment = fragmentIndex[snode-1][0];
//     int dfragment = fragmentIndex[dnode-1][0];
    
//     cout<< "sfragment: " << sfragment << " ,dfragment "<< dfragment<<endl;
    
//     vector<int> sBoundaryNode = boundaryNodeSet[sfragment];
//     vector<int> dBoundaryNode = boundaryNodeSet[dfragment];
    
//     if (sfragment==dfragment){
//         vector<BiobjectivePath> infragmentCostSet = boaPathRetrievalWithInFragment(snode, dnode, sfragment);
//         solutionSet.insert(solutionSet.end(), infragmentCostSet.begin(), infragmentCostSet.end());
//     }
    
//     for (size_t i = 0; i < sBoundaryNode.size(); ++i) {
        
//         sBN = sBoundaryNode[i];
//         for (size_t j = 0; j < dBoundaryNode.size(); ++j){
//             dBN = dBoundaryNode[j];
// //             cout<< "sBN" << sBN << dBN <<endl;
//             vector<BiobjectivePath> onePairBoundaryPathSet = onePairBoundaryPathOf(snode, dnode, sBN, dBN); 
//             solutionSet.insert(solutionSet.end(), onePairBoundaryPathSet.begin(), onePairBoundaryPathSet.end());
            
//         }
//     }

    
//     cout<< "boundary path size: " <<solutionSet.size() <<endl;
    
//     vector<BiobjectivePath> paretoSet = dominanceCheck(solutionSet);
// //     for (size_t j = 0; j < paretoSet.size(); ++j){
// //         cout<< "result: " << paretoSet[j].cost1 <<", " <<paretoSet[j].cost2 <<endl;
// //     }

//     return paretoSet.size();
    
// }

// int HBORBasic::boa(int snode, int dnode){
//     cout<< "snodednode" << snode << dnode <<endl;
//     vector<BiobjectivePath> solutionSet = boaPathRetrievalWithInFragment(snode, dnode, -1);
// //     for (size_t j = 0; j < solutionSet.size(); ++j){
// //         cout<< "result: " << solutionSet[j].cost1 <<", " <<solutionSet[j].cost2 <<endl;
// //     }

//     return solutionSet.size();
    
// }

int HBORBasic::namor(int snode, int dnode){
    cout<< "snodednode" << snode << dnode <<endl;
    vector<BiobjectivePath> solutionSet = namorPathRetrievalWithInFragment(snode, dnode, -1);
//     for (size_t j = 0; j < solutionSet.size(); ++j){
//         cout<< "result: " << solutionSet[j].cost1 <<", " <<solutionSet[j].cost2 <<endl;
//     }

    return solutionSet.size();
    
}


void HBORBasic::cleanupGraphDataCpp(GraphData* graphData) {
    for (int i = 0; i < graphData->numOfArcs; i++) {
        delete[] graphData->edgeVectors[i];
    }
    delete[] graphData->edgeVectors;
}

void HBORBasic::freeGraphDataVector(){
    for (GraphData currentGraph :graphDataVector){
        cleanupGraphDataCpp(&currentGraph);
    }
}