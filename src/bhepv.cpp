#include "bhepv.h"
#include <string>
#include <sstream>

using json = nlohmann::json;
using namespace std;

BHEPV::BHEPV(const std::string& map, int npar) : mapName(map),nPartitions(npar) {
    cout<< "reading" <<endl;
    // read fragment index
    fileFolderName = "../bhepv/"+mapName;
    
}

void BHEPV::readOriginGraph(){
    string filenameEntirGraph = "../Maps/"+mapName+"-road-d.txt";
   
    GraphData graphData;
    int num_nodes = 0;
    int num_arcs = 0;

    // Initialize graphData
    initializeGraphData(&graphData, num_nodes, num_arcs);
    
    int numEdges;
    file >> numVertices >> numEdges;
    // Read data from file and assign it to graphData
    readDataFromFile(&graphData, filenameEntirGraph);

    graphDataVector.push_back(graphData);


}


void BHEPV::readDataFromFile(GraphData* graphData, const std::string& filename) {
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



// Function to read the partition information from a file
void BHEPV::readPartition() {
    string partitionFile = fileFolderName+"/kaffpaIndex.txt"
    std::ifstream inputFile(partitionFile);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Failed to open partition file: " + partitionFile);
    }

    partitions.resize(numVertices)

    for (int i = 0; i < numVertices; ++i) {
        int partition;
        inputFile >> partition;
        partitions[i] = partition;
    }

    inputFile.close();
}


void BHEPV::updateBoundaryNodes() const {  
    GraphData origin = graphDataVector[0];
//     cout<< "get current graph" << currentGraph.numOfGnode << " , "<< currentGraph.numOfArcs << endl;
    const GraphData* graphDataPtr = &origin;
    
    for (int i = 0; i < graphDataPtr->numOfArcs; ++i) {
        int sNode_index = graphDataPtr->edgeVectors[i][0] -1;
        int dNode_index = graphDataPtr->edgeVectors[i][1] -1;
        // Check if the endpoints belong to different fragments
        if (partitions[sNode_index] != partitions[dNode_index]) {
            // Add the endpoints to the boundary nodes set
            boundaryNodes.insert(sNode_index);
            boundaryNodes.insert(dNode_index);
        }
    }
}

void B3HEPV::Precomputation(){
    readOriginGraph();
    readPartition();
    updateBoundaryNodes();
    // generating fragment graphs
    // bod for all boundaryNode within fragment => output 
    // bod for all boundaryNodes in boundary multigraph
}

void B3HEPV::save(){
    // Create a JSON object
    json root;
    std::string boundaryNodePartitionFilename = fileFolderName + "/boundaryNodePartition.json";
    // Convert the boundary nodes and psartitions to JSON
    for (int node : boundaryNodes) {
        int partition = partitions[node];

        // Convert the node and partition to strings
        std::stringstream ssNode;
        ssNode << node+1;
        std::string strNode = ssNode.str();

        // Add the node and partition to the JSON object
        root[strNode] = partition;
    }

    // Open the file for writing
    std::ofstream bnpfile(boundaryNodePartitionFilename);

    if (bnpfile.is_open()) {
        // Write the JSON object to the file
        bnpfile << root.dump();  // Add pretty-printing with indentation level 4
        bnpfile.close();
        std::cout << "Boundary nodes saved to " << boundaryNodePartitionFilename << std::endl;
    } else {
        std::cerr << "Failed to open file: " << boundaryNodePartitionFilename << std::endl;
    }
    
    
    

    // Create a mapping of vertex ID to vertex ID in fragment
    std::unordered_map<int, int> vertexIDInFragment;
    
    
    std::vector<std::unordered_set<int>> fragmentBoundaryNodes(partitionCount);

    // Iterate over the boundary nodes and assign them to their corresponding fragment
    for (int boundaryNode : boundaryNodes) {
        fragmentBoundaryNodes[partitions[boundaryNode]].insert(boundaryNode);
    }
    std::string boundaryFilename = outputFolderName + "/boundaryNodes.txt";
    std::ofstream boundaryFile(boundaryFilename); 
    
    
    // Save each fragment as a separate file
    for (int fragment = 0; fragment < partitionCount; ++fragment) {
        std::string filename = outputFolderName + "/fragments/fragment" + std::to_string(fragment) + ".txt";
        std::ofstream outputFile(filename);
        
        
        const auto& fragBoundaryNodeSet = fragmentBoundaryNodes[fragment];
        for (int boundaryNode : fragBoundaryNodeSet) {
            boundaryFile << boundaryNode+1 << " ";  // Save the original node ID
        }
        boundaryFile << std::endl;

        

        // Count the number of vertices and edges in the fragment
        int numFragmentVertices = 0;
        int numFragmentEdges = 0;

        for (int vertex = 0; vertex < numVertices; ++vertex) {
            if (partitions[vertex] == fragment) {
                vertexIDInFragment[vertex] = ++numFragmentVertices;
                for (const auto& edge : adjacencyList[vertex]) {
                    if (partitions[edge.end] == fragment) {
                        ++numFragmentEdges;
                    }
                }
            }
        }

        // Write the number of vertices and edges in the fragment
        outputFile << numFragmentVertices << " " << numFragmentEdges << std::endl;

        // Write the edges in the fragment
        for (int vertex = 0; vertex < numVertices; ++vertex) {
            if (partitions[vertex] == fragment) {
                int startVertexIDInFragment = vertexIDInFragment[vertex];

                for (const auto& edge : adjacencyList[vertex]) {
                    int endVertex = edge.end;

                    if (partitions[endVertex] == fragment) {
                        int endVertexIDInFragment = vertexIDInFragment[endVertex];

                        // Write the edge information: start node, end node, cost1, cost2
                        outputFile << startVertexIDInFragment << " " << endVertexIDInFragment << " " << edge.cost1 << " " << edge.cost2 << std::endl;
                    }
                }
            }
        }

        outputFile.close();
    }

    // Save the fragment index file
    std::string indexFilename = outputFolderName + "/fragmentIndex.txt";
    std::ofstream indexFile(indexFilename);

    for (int vertex = 0; vertex < numVertices; ++vertex) {
        int fragmentID = partitions[vertex];
        int idInFragment = vertexIDInFragment.at(vertex);
        indexFile << fragmentID << " " << idInFragment << std::endl;
    }

    indexFile.close();
    
    // Perform one-to-all Dijkstra for each boundary node within a fragment and save the results
//     nlohmann::json fragmentResults;
    
    std::string boundaryDijkstraFile = outputFolderName + "/fragmentEncodedPathView.txt";
    std::ofstream boundaryDijkstraOutput(boundaryDijkstraFile);
    
    std::string adjacentLUBs = outputFolderName + "/adjacent_LUBs.txt";
    std::ofstream adjacentLUBsOutput(adjacentLUBs);

    for (int fragment = 0; fragment < partitionCount; ++fragment) {
        
        // Create a separate BiobjectiveGraph for the fragment
        BiobjectiveGraph fragmentGraph(0);

        // Add the vertices and edges within the fragment to the fragmentGraph
        for (int vertex = 0; vertex < numVertices; ++vertex) {
            if (partitions[vertex] == fragment) {
                fragmentGraph.addVertex();
            } 
        }
           
        
        for (int vertex = 0; vertex < numVertices; ++vertex) {
            if (partitions[vertex] == fragment) {
                for (const auto& edge : adjacencyList[vertex]) {
                    int endVertex = edge.end;
                    if (partitions[endVertex] == fragment) {
                       fragmentGraph.addEdge(vertexIDInFragment[vertex], vertexIDInFragment[endVertex], edge.cost1, edge.cost2);

                    }
                }
            }
        }


        // Find the boundary nodes within the fragment
        std::vector<int> boundaryNodesVector;
        for (int vertex = 0; vertex < numVertices; ++vertex) {
            if (partitions[vertex] == fragment && isBoundaryNode(vertex)) {
                boundaryNodesVector.push_back(vertex);
            }
        }
        
        
        
        // Perform one-to-all Dijkstra for each boundary node and save the results
        for (int boundaryNode : boundaryNodesVector) {
            int startVertexIDInFragment = vertexIDInFragment[boundaryNode]-1;

            std::pair<std::vector<int>, std::vector<int>> dijkstraResult1 = fragmentGraph.oneToAllDijkstra(startVertexIDInFragment, 0);
            std::pair<std::vector<int>, std::vector<int>> dijkstraResult2 = fragmentGraph.oneToAllDijkstra(startVertexIDInFragment, 1);
             
            // Store the results in the JSON object
            for (int target = 0; target < numVertices; ++target) {
                if (partitions[target] == fragment) {
                    int endVertexIDInFragment = vertexIDInFragment[target];
                    int minCostComponent1 = dijkstraResult1.first[endVertexIDInFragment - 1];
                    
                    int minOtherCostComponent1 = dijkstraResult1.second[endVertexIDInFragment - 1];
                    int minCostComponent2 = dijkstraResult2.first[endVertexIDInFragment - 1];
                    int minOtherCostComponent2 = dijkstraResult2.second[endVertexIDInFragment - 1];

//                     // Add the vertex results to the fragment results
//                     std::ostringstream ossBoundaryNode;
//                     ossBoundaryNode << boundaryNode+1;
//                     std::ostringstream ossTarget;
//                     ossTarget << target+1;
//                     fragmentResults[ossBoundaryNode.str()][ossTarget.str()] = { minCostComponent1, minCostComponent2, minOtherCostComponent2, minOtherCostComponent1};
                    
                     // Write the results to the file
                    boundaryDijkstraOutput << boundaryNode+1 << " " << target+1 << " " << minCostComponent1 << " " << minCostComponent2 << " "
                               << minOtherCostComponent2 << " " << minOtherCostComponent1 << std::endl;
                    if (isBoundaryNode(target) && target != boundaryNode){
                        adjacentLUBsOutput << boundaryNode+1 << " " << target+1 << " " << minCostComponent1 << " " << minCostComponent2 << " "
                               << minOtherCostComponent2 << " " << minOtherCostComponent1 << std::endl;
                    }
                    
                }
            }
            for (const auto& edge : adjacencyList[boundaryNode]) {
                int target = edge.end;
                // Check if the endpoints belong to different fragments
                if (partitions[boundaryNode] != partitions[target]) {
                    adjacentLUBsOutput << boundaryNode+1 << " " << target+1 << " " << edge.cost1 << " " << edge.cost2 << " "
                               << edge.cost1 << " " << edge.cost2 << std::endl;
                }
            }
            
        }
        
    }
    
    std::cout<< "successful encoded fragment path view" <<std::endl;
    
//     std::string boundaryDijkstraFile = outputFolderName + "/fragmentEncodedPathView.json";
//     std::ofstream boundaryDijkstraOutput(boundaryDijkstraFile);
//     boundaryDijkstraOutput << fragmentResults.dump(4); // Use pretty printing with indentation of 4 spaces
    boundaryDijkstraOutput.close();
    adjacentLUBsOutput.close();
}

void B3HEPV::load(){
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

void B3HEPV::hbor(int snode){

}


void BiobjectiveGraph::saveFragments() const {
    // Create a JSON object
    json root;
    std::string boundaryNodePartitionFilename = outputFolderName + "/boundaryNodePartition.json";
    // Convert the boundary nodes and psartitions to JSON
    for (int node : boundaryNodes) {
        int partition = partitions[node];

        // Convert the node and partition to strings
        std::stringstream ssNode;
        ssNode << node+1;
        std::string strNode = ssNode.str();

        // Add the node and partition to the JSON object
        root[strNode] = partition;
    }

    // Open the file for writing
    std::ofstream bnpfile(boundaryNodePartitionFilename);

    if (bnpfile.is_open()) {
        // Write the JSON object to the file
        bnpfile << root.dump(4);  // Add pretty-printing with indentation level 4
        bnpfile.close();
        std::cout << "Boundary nodes saved to " << boundaryNodePartitionFilename << std::endl;
    } else {
        std::cerr << "Failed to open file: " << boundaryNodePartitionFilename << std::endl;
    }
    
    
    

    // Create a mapping of vertex ID to vertex ID in fragment
    std::unordered_map<int, int> vertexIDInFragment;
    
    
    std::vector<std::unordered_set<int>> fragmentBoundaryNodes(partitionCount);

    // Iterate over the boundary nodes and assign them to their corresponding fragment
    for (int boundaryNode : boundaryNodes) {
        fragmentBoundaryNodes[partitions[boundaryNode]].insert(boundaryNode);
    }
    std::string boundaryFilename = outputFolderName + "/boundaryNodes.txt";
    std::ofstream boundaryFile(boundaryFilename); 
    
    
    // Save each fragment as a separate file
    for (int fragment = 0; fragment < partitionCount; ++fragment) {
        std::string filename = outputFolderName + "/fragments/fragment" + std::to_string(fragment) + ".txt";
        std::ofstream outputFile(filename);
        
        
        const auto& fragBoundaryNodeSet = fragmentBoundaryNodes[fragment];
        for (int boundaryNode : fragBoundaryNodeSet) {
            boundaryFile << boundaryNode+1 << " ";  // Save the original node ID
        }
        boundaryFile << std::endl;

        

        // Count the number of vertices and edges in the fragment
        int numFragmentVertices = 0;
        int numFragmentEdges = 0;

        for (int vertex = 0; vertex < numVertices; ++vertex) {
            if (partitions[vertex] == fragment) {
                vertexIDInFragment[vertex] = ++numFragmentVertices;
                for (const auto& edge : adjacencyList[vertex]) {
                    if (partitions[edge.end] == fragment) {
                        ++numFragmentEdges;
                    }
                }
            }
        }

        // Write the number of vertices and edges in the fragment
        outputFile << numFragmentVertices << " " << numFragmentEdges << std::endl;

        // Write the edges in the fragment
        for (int vertex = 0; vertex < numVertices; ++vertex) {
            if (partitions[vertex] == fragment) {
                int startVertexIDInFragment = vertexIDInFragment[vertex];

                for (const auto& edge : adjacencyList[vertex]) {
                    int endVertex = edge.end;

                    if (partitions[endVertex] == fragment) {
                        int endVertexIDInFragment = vertexIDInFragment[endVertex];

                        // Write the edge information: start node, end node, cost1, cost2
                        outputFile << startVertexIDInFragment << " " << endVertexIDInFragment << " " << edge.cost1 << " " << edge.cost2 << std::endl;
                    }
                }
            }
        }

        outputFile.close();
    }

    // Save the fragment index file
    std::string indexFilename = outputFolderName + "/fragmentIndex.txt";
    std::ofstream indexFile(indexFilename);

    for (int vertex = 0; vertex < numVertices; ++vertex) {
        int fragmentID = partitions[vertex];
        int idInFragment = vertexIDInFragment.at(vertex);
        indexFile << fragmentID << " " << idInFragment << std::endl;
    }

    indexFile.close();
    
    // Perform one-to-all Dijkstra for each boundary node within a fragment and save the results
//     nlohmann::json fragmentResults;
    
    std::string boundaryDijkstraFile = outputFolderName + "/fragmentEncodedPathView.txt";
    std::ofstream boundaryDijkstraOutput(boundaryDijkstraFile);
    
    std::string adjacentLUBs = outputFolderName + "/adjacent_LUBs.txt";
    std::ofstream adjacentLUBsOutput(adjacentLUBs);

    for (int fragment = 0; fragment < partitionCount; ++fragment) {
        
        // Create a separate BiobjectiveGraph for the fragment
        BiobjectiveGraph fragmentGraph(0);

        // Add the vertices and edges within the fragment to the fragmentGraph
        for (int vertex = 0; vertex < numVertices; ++vertex) {
            if (partitions[vertex] == fragment) {
                fragmentGraph.addVertex();
            } 
        }
           
        
        for (int vertex = 0; vertex < numVertices; ++vertex) {
            if (partitions[vertex] == fragment) {
                for (const auto& edge : adjacencyList[vertex]) {
                    int endVertex = edge.end;
                    if (partitions[endVertex] == fragment) {
                       fragmentGraph.addEdge(vertexIDInFragment[vertex], vertexIDInFragment[endVertex], edge.cost1, edge.cost2);

                    }
                }
            }
        }


        // Find the boundary nodes within the fragment
        std::vector<int> boundaryNodesVector;
        for (int vertex = 0; vertex < numVertices; ++vertex) {
            if (partitions[vertex] == fragment && isBoundaryNode(vertex)) {
                boundaryNodesVector.push_back(vertex);
            }
        }
        
        
        
        // Perform one-to-all Dijkstra for each boundary node and save the results
        for (int boundaryNode : boundaryNodesVector) {
            int startVertexIDInFragment = vertexIDInFragment[boundaryNode]-1;

            std::pair<std::vector<int>, std::vector<int>> dijkstraResult1 = fragmentGraph.oneToAllDijkstra(startVertexIDInFragment, 0);
            std::pair<std::vector<int>, std::vector<int>> dijkstraResult2 = fragmentGraph.oneToAllDijkstra(startVertexIDInFragment, 1);
             
            // Store the results in the JSON object
            for (int target = 0; target < numVertices; ++target) {
                if (partitions[target] == fragment) {
                    int endVertexIDInFragment = vertexIDInFragment[target];
                    int minCostComponent1 = dijkstraResult1.first[endVertexIDInFragment - 1];
                    
                    int minOtherCostComponent1 = dijkstraResult1.second[endVertexIDInFragment - 1];
                    int minCostComponent2 = dijkstraResult2.first[endVertexIDInFragment - 1];
                    int minOtherCostComponent2 = dijkstraResult2.second[endVertexIDInFragment - 1];

//                     // Add the vertex results to the fragment results
//                     std::ostringstream ossBoundaryNode;
//                     ossBoundaryNode << boundaryNode+1;
//                     std::ostringstream ossTarget;
//                     ossTarget << target+1;
//                     fragmentResults[ossBoundaryNode.str()][ossTarget.str()] = { minCostComponent1, minCostComponent2, minOtherCostComponent2, minOtherCostComponent1};
                    
                     // Write the results to the file
                    boundaryDijkstraOutput << boundaryNode+1 << " " << target+1 << " " << minCostComponent1 << " " << minCostComponent2 << " "
                               << minOtherCostComponent2 << " " << minOtherCostComponent1 << std::endl;
                    if (isBoundaryNode(target) && target != boundaryNode){
                        adjacentLUBsOutput << boundaryNode+1 << " " << target+1 << " " << minCostComponent1 << " " << minCostComponent2 << " "
                               << minOtherCostComponent2 << " " << minOtherCostComponent1 << std::endl;
                    }
                    
                }
            }
            for (const auto& edge : adjacencyList[boundaryNode]) {
                int target = edge.end;
                // Check if the endpoints belong to different fragments
                if (partitions[boundaryNode] != partitions[target]) {
                    adjacentLUBsOutput << boundaryNode+1 << " " << target+1 << " " << edge.cost1 << " " << edge.cost2 << " "
                               << edge.cost1 << " " << edge.cost2 << std::endl;
                }
            }
            
        }
        
    }
    
    std::cout<< "successful encoded fragment path view" <<std::endl;
    
//     std::string boundaryDijkstraFile = outputFolderName + "/fragmentEncodedPathView.json";
//     std::ofstream boundaryDijkstraOutput(boundaryDijkstraFile);
//     boundaryDijkstraOutput << fragmentResults.dump(4); // Use pretty printing with indentation of 4 spaces
    boundaryDijkstraOutput.close();
    adjacentLUBsOutput.close();
}








































