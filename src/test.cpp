#include <cassert>
#include <vector>
#include <list>
#include <array>
#include <iostream>  
#include <chrono> // Include the chrono library
#include "hbor.h"
#include "biobjectiveGraph.h"
#include "dfs_class_v2.h"
using namespace std;

// module load gcc/11.3.0
// gcc -std=c99 -o test.o test.cpp hbor.cpp biobjectiveGraph.cpp pathRetrieval.c heap.c boastar.c graph.c -lstdc++
// ./test.o

#if 0
void test_readFragementIndex() {
    // tested
    B3HEPV hepv = B3HEPV("../b3hepv");
    vector<array<int, 2>>  result = hepv.fragmentIndex;
    for (size_t i = 0; i < result.size(); ++i) {
        cout << "For node" << i+1 << ": Fragment ID "<< result[i][0] << ", Fragment node ID: "<< result[i][1] <<"\n";
        
    }
    cout<<endl;
    // assert(result == expected);  // Assertion using assert macro
    // cout << "Test passed!" << endl;
}

void test_readBoundaryNode(){
    // tested
    B3HEPV hepv = B3HEPV("../b3hepv");
    vector<vector<int>> matrix = hepv.boundaryNodeSet;
    cout<< "start";
    for (const auto& row : matrix) {
        for (int num : row) {
            cout << num << " ";
        }
        cout << endl;
    }

}

void test_boundaryPathDominanceCheck(){
    // tested
    vector<int> lub1 = {10, 20, 10, 20};
    vector<int> path1 = {5, 6, 7, 8};
    BoundaryPath bp1(lub1, path1);
    
    vector<int> lub2 = {20, 10, 40, 30};
    vector<int> path2 = {5, 14, 15, 8};
    BoundaryPath bp2(lub2, path2);

    vector<int> lub3 = {10 , 20, 20, 30};
    vector<int> path3 = {51, 410, 115, 18};
    BoundaryPath bp3(lub3, path3);

    vector<int> lub4 = {30 , 40, 50, 70};
    vector<int> path4 = {50, 400, 105, 80};
    BoundaryPath bp4(lub4, path4);

    vector<BoundaryPath> boundaryPathSet = {bp1, bp2, bp3, bp4};

    cout<< "before:" <<endl;

    for (const BoundaryPath& boundaryPath: boundaryPathSet) {
        boundaryPath.printPath();
    }

    B3HEPV hepv = B3HEPV("../b3hepv/test");

    vector<BoundaryPath> paretoBoundaryPath = hepv.boundaryPathDominanceCheck(boundaryPathSet);
    cout<< "after:" <<endl;
    for (const BoundaryPath& boundaryPath: paretoBoundaryPath) {
        boundaryPath.printPath();
    }
}

void test_ReadBoundaryPathView(){
    // tested
    B3HEPV hepv = B3HEPV("../b3hepv");
    for (const auto& entry1 : hepv.boundaryEncodedPathView) {
        int key1 = entry1.first;
        for (const auto& entry2 : entry1.second) {
            int key2 = entry2.first;
            const vector<BoundaryPath>& paths = entry2.second;

            cout << "snode: " << key1 << ",dnode : " << key2 << endl;
            for (const auto& path : paths) {
                path.printPath();
            }
        }
    }
}

void test_concatBoundaryPath(){
    // tested
    BoundaryPath path1({1, 2, 3, 4}, {10, 20, 30, 40, 50});
    BoundaryPath path2({5, 6, 7, 8}, {50, 60, 70, 80});
    BoundaryPath path3({5, 6, 7, 8}, {80, 90, 100});

    try {
        BoundaryPath concatenatedPath = path1.concatWith(path2).concatWith(path3);
        cout << "Concatenated Lubs: ";
        for (int i : concatenatedPath.lub) {
            cout << i << " ";
        }
        cout << endl;
        concatenatedPath.printPath();
    } catch (const runtime_error& error) {
        cout << "Error: " << error.what() << endl;
    }
}

void test_readFragmentEncodedPathView(){
    // tested
    std::cout<< "testing test_readFragmentEncodedPathView"<<std::endl;
    B3HEPV hepv = B3HEPV("../b3hepv/test");
    for (const auto& entry : hepv.fragmentEncodedPathView) {
        int key1 = entry.first;
        for (const auto& innerEntry : entry.second) {
            int key2 = innerEntry.first;
            const BoundaryPath& boundaryPath = innerEntry.second;
            boundaryPath.printPath();
            for (const auto& cost : boundaryPath.lub) {
                cout<<" "<<cost<<" ";
            }
        }
    }
    cout<<endl;
}

void test_readAdjacentLub(){
    // tested
    B3HEPV hepv = B3HEPV("../b3hepv/test");
    for (const auto& entry : hepv.adjacentLub) {
        int key1 = entry.first;

        for (const auto& innerEntry : entry.second) {
            int key2 = innerEntry.first;
            const std::vector<int> costs = innerEntry.second;
            cout<<"snode "<<key1<<"dnode" << key2 <<endl;
            for (const auto& cost : costs) {
                cout<<" "<<cost<<" ";
            }
        }
    }
    cout <<endl;
}

void test_onePairBoundaryPathOf(){
    // tested
    B3HEPV hepv = B3HEPV("../b3hepv");
    vector<BoundaryPath> boundaryPathSet = hepv.onePairBoundaryPathOf(1, 3, 4, 5);
    for (const auto& path : boundaryPathSet) {
        for (int i : path.lub) {
            cout << i << " ";
        }
        cout << endl;
        path.printPath();
    }
}

void test_paretoBoundaryPathBetween(){
    // tested
    B3HEPV hepv = B3HEPV("../b3hepv/test");
    vector<BoundaryPath> boundaryPathSet = hepv.paretoBoundaryPathBetween(8, 1);
    for (const auto& path : boundaryPathSet) {
        for (int i : path.lub) {
            cout << i << " ";
        }
        cout << endl;
        path.printPath();
    }
}

void test_combineCostSet(){
    // tested
    B3HEPV hepv = B3HEPV("../b3hepv");
    std::vector<Solution> costSet1;
    costSet1.push_back(Solution(1, 2));
    costSet1.push_back(Solution(3, 4));

    std::vector<Solution> costSet2;
    costSet2.push_back(Solution(5, 6));
    costSet2.push_back(Solution(7, 8));

    // Combine the sets
    std::vector<Solution> combinedSet = hepv.combineCostSet(costSet1, costSet2);

    // Print the combined solutions
    for (const Solution& solution : combinedSet) {
        std::cout << "Combined Solution: (" << solution.cost1 << ", " << solution.cost2 << ")" << std::endl;
    }

}

void test_dominanceCheck(){
    // tested
    // Create a vector of Solution objects
    std::vector<Solution> superParetoCostSet;
    superParetoCostSet.push_back(Solution(1, 2));
    superParetoCostSet.push_back(Solution(3, 4));
    superParetoCostSet.push_back(Solution(2, 1));
    superParetoCostSet.push_back(Solution(4, 5));
    superParetoCostSet.push_back(Solution(3, 6));
    B3HEPV hepv = B3HEPV("../b3hepv");
    // Perform dominance check
    std::vector<Solution> nonDominatedSolutions = hepv.dominanceCheck(superParetoCostSet);

    // Print the non-dominated solutions
    for (const Solution& solution : nonDominatedSolutions) {
        std::cout << "Non-Dominated Solution: (" << solution.cost1 << ", " << solution.cost2 << ")" << std::endl;
    }
}

void test_pathRetrievalWithInFragment(){
    // tested
    B3HEPV hepv = B3HEPV("../b3hepv");
    vector<Solution> pathCostSet2 = hepv.pathRetrievalWithInFragment(1, 4, 0);
    cout<< "size: "<<pathCostSet2.size() << endl;
    for (const Solution& solution : pathCostSet2) {
        std::cout << "Solutions: (" << solution.cost1 << ", " << solution.cost2 << ")" << std::endl;
    }
    vector<Solution> pathCostSet = hepv.pathRetrievalWithInFragment(6, 8, 2);
    cout<< "size: "<<pathCostSet.size() << endl;
    for (const Solution& solution : pathCostSet) {
        std::cout << "Solutions: (" << solution.cost1 << ", " << solution.cost2 << ")" << std::endl;
    }
    
}



void test_expandPathCostOf(){
    // tested
    B3HEPV hepv = B3HEPV("../b3hepv");
    BoundaryPath bp = BoundaryPath({10, 10, 15, 15},{1, 4, 6, 8});
    vector<Solution> pathCostSet = hepv.pathRetrievalWithInFragment(6, 8, 2);
    hepv.adjacentLub[4][6] = {5,6,5,6};
    vector<Solution> solutions = hepv.expandPathCostOf(bp);
    for (const Solution& solution : solutions) {
        std::cout << "Solutions: (" << solution.cost1 << ", " << solution.cost2 << ")" << std::endl;
    }
}

void test_expandPathForBoundaryPathSet(){
    // tested
    B3HEPV hepv = B3HEPV("../b3hepv");
    vector<BoundaryPath> bpSet;
    BoundaryPath bp = BoundaryPath({10, 10, 15, 15},{1, 4, 6, 8});
    BoundaryPath bp2 = BoundaryPath({10, 10, 15, 15},{1, 3, 5, 7, 8});
    bpSet.push_back(bp);
    bpSet.push_back(bp2);
    hepv.adjacentLub[4][6] = {5,6,5,6};
    hepv.adjacentLub[1][3] = {7,8,7,8};
    hepv.adjacentLub[5][7] = {9,7,9,7};
    vector<Solution> solutions = hepv.expandPathForBoundaryPathSet(bpSet);
    for (const Solution& solution : solutions) {
        std::cout << "Solutions: (" << solution.cost1 << ", " << solution.cost2 << ")" << std::endl;
    }
}




// void test_boudnaryDFS(){
//     string adjacentLub = "../b3hepv/adjacent_LUBs.txt";
//     string existingLub = "../b3hepv/existingLUB.txt";
//     Bi_objective_DFS bodfs(adjacentLub, existingLub);
//     bodfs.allPairs();
//     bodfs.save_all_path("json");
// }

void test_biobjectiveGraph(){
    //tested
    BiobjectiveGraph graph(5);  // Create a graph with 5 vertices

    // Add edges to the graph
    graph.addEdge(0, 1, 3, 5);
    graph.addEdge(0, 2, 2, 6);
    graph.addEdge(1, 2, 1, 2);
    graph.addEdge(1, 3, 4, 1);
    graph.addEdge(2, 3, 2, 4);
    graph.addEdge(2, 4, 1, 3);
    graph.addEdge(3, 4, 2, 2);

    int source = 0;
    int costComponent = 1;  // Specify the cost component (0 or 1)

    std::pair<std::vector<int>, std::vector<int>> result = graph.oneToAllDijkstra(source, costComponent);

    // Display the minimum value of the specified cost component and the corresponding value of the other cost component
    for (int i = 0; i < result.first.size(); i++) {
        std::cout << "Vertex " << i << ": Cost" << costComponent << " = " << result.first[i] << ", Other Cost = " << result.second[i] << std::endl;
    }
}


void test_biobjectiveGraphFromFile(){
    // tested
    //BiobjectiveGraph graph("../Maps/COL-road-d.txt"); 
    BiobjectiveGraph graph("../Maps/testImputGraph.txt");
    std::cout<<"number of vertices: "<< graph.numVertices <<std::endl;
    int source = 0;
    int costComponent = 0;  // Specify the cost component (0 or 1)
    std::pair<std::vector<int>, std::vector<int>> result = graph.oneToAllDijkstra(source, costComponent);
    // Display the minimum value of the specified cost component and the corresponding value of the other cost component
    for (int i = 0; i < result.first.size(); i++) {
        std::cout << "Vertex " << i << ": Cost" << costComponent << " = " << result.first[i] << ", Other Cost = " << result.second[i] << std::endl;
    }
    std::cout<<"finished"<<std::endl;
}

void test_biobjectiveGraphDijkstra(){
    // tested
    auto startTime = std::chrono::high_resolution_clock::now();
    BiobjectiveGraph graph("../Maps/COL-road-d.txt"); 
    std::cout<<"number of vertices: "<< graph.numVertices <<std::endl;
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    // Print the elapsed time
    std::cout << "Elapsed time: " << duration.count() << " milliseconds" << std::endl;
    int source = 0;
    int costComponent = 0;  // Specify the cost component (0 or 1)
    
    startTime = std::chrono::high_resolution_clock::now();
    std::pair<std::vector<int>, std::vector<int>> result = graph.oneToAllDijkstra(source, costComponent);
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    // Print the elapsed time
    std::cout << "Elapsed time: " << duration.count() << " milliseconds" << std::endl;
}

void test_readPartition(){
    // tested
    // BiobjectiveGraph graph("../Maps/testImputGraph.txt");
    BiobjectiveGraph graph("../Maps/COL-road-d.txt");
    try {
        // Read the partition information from a file
        graph.readPartition("../b3hepv/col/kaffpaIndex.txt");

        // Print the partition information
//         std::cout << "Partition Information:" << std::endl;
//         for (int vertex = 0; vertex < graph.numVertices; ++vertex) {
//             std::cout << "Vertex " << vertex << ": Partition " << graph.partitions[vertex] << std::endl;
//         }
        std::cout << "Partition Count " << graph.partitionCount << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void test_saveFragments(){
    // tested
    BiobjectiveGraph graph("../Maps/testImputGraph.txt");
    // BiobjectiveGraph graph("../Maps/COL-road-d.txt");
    try {
        // Read the partition information from a file
        graph.readPartition("../b3hepv/test/kaffpaIndex.txt");
        std::cout << "Partition Count " << graph.partitionCount << std::endl;
        std::string outputFolderName = "../b3hepv/test/fragments";
        graph.saveFragments(outputFolderName);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void test_saveFragmentsLarge(){
    // tested
    BiobjectiveGraph graph("../Maps/COL-road-d.txt");
    try {
        // Read the partition information from a file
        graph.readPartition("../b3hepv/col/kaffpaIndex.txt");
        std::cout << "Partition Count " << graph.partitionCount << std::endl;
        std::string outputFolderName = "../b3hepv/col/fragments";
        graph.saveFragments(outputFolderName);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void test_findBoundaryNodes(){
    // tested
    BiobjectiveGraph graph("../Maps/testImputGraph.txt");
    try {
        // Read the partition information from a file
        graph.readPartition("../b3hepv/test/kaffpaIndex.txt");
        std::cout << "Partition Count " << graph.partitionCount << std::endl;
        // Find the boundary nodes
        std::unordered_set<int> boundaryNodes = graph.findBoundaryNodes();

        // Print the boundary nodes
        std::cout << "Boundary Nodes: ";
        for (int node : boundaryNodes) {
            std::cout << node << " ";
        }
        std::cout << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void test_findBoundaryNodesLarge(){
    // tested
    BiobjectiveGraph graph("../Maps/COL-road-d.txt");
    try {
        // Read the partition information from a file
        graph.readPartition("../b3hepv/col/kaffpaIndex.txt");
        std::cout << "Partition Count " << graph.partitionCount << std::endl;
        // Find the boundary nodes
        std::unordered_set<int> boundaryNodes = graph.findBoundaryNodes();

        // Print the boundary nodes
        std::cout << "Boundary Nodes: ";
        int i = 0;
        for (int node : boundaryNodes) {
            i+=1;
        }
        std::cout <<"No. BoundaryNodes: "<<i<< std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void test_fragmentEncodedPathView(){
    // tested
    BiobjectiveGraph graph("../Maps/testImputGraph.txt");
    try {
        // Read the partition information from a file
        graph.readPartition("../b3hepv/test/kaffpaIndex.txt");
        graph.updateBoundaryNodes();
        std::cout << "Partition Count " << graph.partitionCount << std::endl;
        // Print the boundary nodes
        std::cout << "Boundary Nodes: ";
        for (int node : graph.boundaryNodes) {
            std::cout << node << " ";
        }
        std::string outputFolderName = "../b3hepv/test";
        graph.saveFragments(outputFolderName);
        std::cout << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}


void test_fragmentEncodedPathViewLarge(){
    // tested
    BiobjectiveGraph graph("../Maps/COL-road-d.txt");
    try {
        // Read the partition information from a file
        graph.readPartition("../b3hepv/col/kaffpaIndex.txt");
        graph.updateBoundaryNodes();
        std::cout << "Partition Count " << graph.partitionCount << std::endl;
        std::string outputFolderName = "../b3hepv/col";
        graph.saveFragments(outputFolderName);
        std::cout << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}


void test_existingLUB(){
    // tested
    BiobjectiveGraph graph("../Maps/testImputGraph.txt");
    try {
        // Read the partition information from a file
        graph.readPartition("../b3hepv/test/kaffpaIndex.txt");
        graph.updateBoundaryNodes();
        std::cout << "Partition Count " << graph.partitionCount << std::endl;
        // Print the boundary nodes
        std::cout << "Boundary Nodes: ";
        for (int node : graph.boundaryNodes) {
            std::cout << node << " ";
        }
        std::string outputFolderName = "../b3hepv/test";
        graph.saveFragments(outputFolderName);
        graph.saveExistingLUB(outputFolderName);
        std::cout << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void test_existingLUBLarge(){
    // tested
    BiobjectiveGraph graph("../Maps/COL-road-d.txt");
    try {
        auto startTime = std::chrono::high_resolution_clock::now();
        // Read the partition information from a file
        graph.readPartition("../b3hepv/col/kaffpaIndex.txt");
        graph.updateBoundaryNodes();
        std::cout << "Partition Count " << graph.partitionCount << std::endl;
        // Print the boundary nodes
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        std::cout << "Elapsed time of read partition: " << duration.count() << " milliseconds" << std::endl;
        
        std::string outputFolderName = "../b3hepv/col";
        
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

void test_dfs(){
    // tested
    std::string folderName = "../b3hepv/test";
    Bi_objective_DFS bodfs(folderName);
    bodfs.allPairs();
    bodfs.save_all_path("json");
    bodfs.convert_map_of_map_to_json_file();
}

void test_dfsLarge(){
    // untested
    auto startTime = std::chrono::high_resolution_clock::now();
    std::string folderName = "../b3hepv/col";
    Bi_objective_DFS bodfs(folderName);
    bodfs.allPairs();
    bodfs.save_all_path("json");
    bodfs.convert_map_of_map_to_json_file();
    // Print the boundary nodes
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "Elapsed time of dfs: " << duration.count() << " milliseconds" << std::endl;
}

void test_precomputation(){
    auto startTimeTotal = std::chrono::high_resolution_clock::now();
    precomputation("test");
    auto endTimeTotal = std::chrono::high_resolution_clock::now();
    auto durationTotal = std::chrono::duration_cast<std::chrono::milliseconds>(endTimeTotal - startTimeTotal);
    std::cout << "Elapsed time of precomputation: " << durationTotal.count() << " milliseconds" << std::endl;
}
#endif

void test_hbor(){
    B3HEPV hepv = B3HEPV("test",3);
    cout<< hepv.hbor(1,8) <<endl;
    cout<< hepv.hbor(8,1) <<endl;
    cout<< hepv.hbor(1,4) <<endl;
    cout<< hepv.hbor(5,4) <<endl;
    cout<< hepv.hbor(3,6) <<endl;
}


void test_boa(){
    B3HEPV hepv = B3HEPV("test",3);
    cout<< hepv.boaPathRetrievalFromFile(1,6, "../Maps/multigraph-road-d.txt") <<endl;
    cout<< hepv.boaPathRetrievalFromFile(6,1, "../Maps/multigraph-road-d.txt") <<endl;
    cout<< hepv.boaPathRetrievalFromFile(3,2, "../Maps/multigraph-road-d.txt") <<endl;
    cout<< hepv.boaPathRetrievalFromFile(4,5, "../Maps/multigraph-road-d.txt") <<endl;
}


void test_boaFromFile(){
    B3HEPV hepv = B3HEPV("test",3);
    cout<< hepv.boaPathRetrievalFromFile(186399, 206453, "../Maps/COL-road-d.txt") <<endl;
    cout<< hepv.boaPathRetrievalFromFile(106474, 399484, "../Maps/COL-road-d.txt") <<endl;
    cout<< hepv.boaPathRetrievalFromFile(219775, 41597, "../Maps/COL-road-d.txt") <<endl;
    cout<< hepv.boaPathRetrievalFromFile(240731, 182571, "../Maps/COL-road-d.txt") <<endl;
    cout<< hepv.boaPathRetrievalFromFile(417012, 345347, "../Maps/COL-road-d.txt") <<endl;
}

int main() {

    // Run the test functions

    // test_readFragementIndex();
    // test_readBoundaryNode();
    // test_boundaryPathDominanceCheck();
    // test_ReadBoundaryPathView();
    // test_concatBoundaryPath();
    // test_readFragmentEncodedPathView();
    // test_onePairBoundaryPathOf();
    // test_dominanceCheck();
    // test_combineCostSet();
    // test_pathRetrievalWithInFragment();
    // test_expandPathCostOf();
    // test_expandPathForBoundaryPathSet();
    // test_biobjectiveGraph();
    // test_biobjectiveGraphFromFile();
    // test_biobjectiveGraphDijkstra();
    // test_readPartition();
    // test_saveFragments();
    // test_saveFragmentsLarge();
    // test_findBoundaryNodes();
    // test_findBoundaryNodesLarge();
    // test_fragmentEncodedPathView();
    // test_fragmentEncodedPathViewLarge();
    // test_existingLUB();
    // test_existingLUBLarge();
    // test_dfs();
    // test_precomputation();

    // test_readFragmentEncodedPathView();
    // test_readAdjacentLub();
    
    // test_paretoBoundaryPathBetween(); 
//     test_boa();
    // remained untested
    // test_dfsLarge();
    
//     test_hbor();
    test_boaFromFile();
    
    return 0;
}