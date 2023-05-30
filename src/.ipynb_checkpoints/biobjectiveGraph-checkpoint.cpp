#include "biobjectiveGraph.h"
#include <string>

BiobjectiveGraph::BiobjectiveGraph(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    int numEdges;
    file >> numVertices >> numEdges;
    
    adjacencyList.resize(numVertices);
    partitions.resize(numVertices);

    for (int i = 0; i < numEdges; ++i) {
        int start, end, cost1, cost2;
        file >> start >> end >> cost1 >> cost2;
        addEdge(start, end, cost1, cost2);
    }
    file.close();
    std::cout<<"file read"<<std::endl;
}

void BiobjectiveGraph::addEdge(int start, int end, int cost1, int cost2) {
    adjacencyList[start-1].push_back({end-1, cost1, cost2 });
}


std::pair<std::vector<int>, std::vector<int>> BiobjectiveGraph::oneToAllDijkstra(int source, int costComponent) const {
    std::vector<int> distance(numVertices, std::numeric_limits<int>::max());
    std::vector<int> otherCost(numVertices, std::numeric_limits<int>::max());

    auto comparator = [&](const std::tuple<int, int, int>& a, const std::tuple<int, int, int>& b) {
        int costA = std::get<1>(a);
        int costB = std::get<1>(b);
        int otherA = std::get<2>(a);
        int otherB = std::get<2>(b);

        return costB < costA || (costA == costB && otherB < otherA);
    };

    std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, decltype(comparator)> pq(comparator);

    distance[source] = 0;
    otherCost[source] = 0;
    pq.push(std::make_tuple(source, 0, 0));
    
    std::vector<bool> visited(numVertices, false);
    while (!pq.empty()) {
        int current = std::get<0>(pq.top());
        int cost = std::get<1>(pq.top());
        int other = std::get<2>(pq.top());
        pq.pop();
        if (visited[current])
            continue;
        visited[current] = true;

        for (const auto& edge : adjacencyList[current]) {
            int target = edge.end;
            int newCost = (costComponent == 0) ? edge.cost1 : edge.cost2;
            int newOther = (costComponent == 0) ? edge.cost2 : edge.cost1;

            if (distance[current] + newCost < distance[target] || (distance[current] + newCost == distance[target] && other + newOther < otherCost[target])) {
                distance[target] = distance[current] + newCost;
                otherCost[target] = other + newOther;
                pq.push(std::make_tuple(target, distance[target], otherCost[target]));
            }
        }
    }

    return { distance, otherCost };
}

// Function to read the partition information from a file
void BiobjectiveGraph::readPartition(const std::string& partitionFile) {
    std::ifstream inputFile(partitionFile);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Failed to open partition file: " + partitionFile);
    }

    partitions.resize(numVertices);
    partitionCount = 0; // Initialize partitionCount to 0

    for (int i = 0; i < numVertices; ++i) {
        int partition;
        inputFile >> partition;
        partitions[i] = partition;
        // Update partitionCount if necessary
        if (partition > partitionCount) {
            partitionCount = partition;
        }
    }
    partitionCount+=1;

    inputFile.close();
}


std::unordered_set<int> BiobjectiveGraph::findBoundaryNodes() const {
    std::unordered_set<int> boundaryNodes;

    for (int startVertex = 0; startVertex < numVertices; ++startVertex) {
        for (const auto& edge : adjacencyList[startVertex]) {
            int endVertex = edge.end;

            // Check if the endpoints belong to different fragments
            if (partitions[startVertex] != partitions[endVertex]) {
                // Add the endpoints to the boundary nodes set
                boundaryNodes.insert(startVertex);
                boundaryNodes.insert(endVertex);
            }
        }
    }

    return boundaryNodes;
}

void BiobjectiveGraph::addVertex() {
    numVertices++;
    adjacencyList.emplace_back();
}


void BiobjectiveGraph::saveFragments(const std::string& outputFolderName) const {

    // Create a mapping of vertex ID to vertex ID in fragment
    std::unordered_map<int, int> vertexIDInFragment;

    // Save each fragment as a separate file
    for (int fragment = 0; fragment < partitionCount; ++fragment) {
        std::string filename = outputFolderName + "/fragment" + std::to_string(fragment) + ".txt";
        std::ofstream outputFile(filename);

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
        std::vector<int> boundaryNodes;
        for (int vertex = 0; vertex < numVertices; ++vertex) {
            if (partitions[vertex] == fragment && isBoundaryNode(vertex)) {
                boundaryNodes.push_back(vertex);
            }
        }
        
        
        
        // Perform one-to-all Dijkstra for each boundary node and save the results
        for (int boundaryNode : boundaryNodes) {
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
                }
            }
            
        }
        
    }
    
    std::cout<< "successful encoded fragment path view" <<std::endl;
    
//     std::string boundaryDijkstraFile = outputFolderName + "/fragmentEncodedPathView.json";
//     std::ofstream boundaryDijkstraOutput(boundaryDijkstraFile);
//     boundaryDijkstraOutput << fragmentResults.dump(4); // Use pretty printing with indentation of 4 spaces
    boundaryDijkstraOutput.close();
}




