#include <cassert>
#include <vector>
#include <list>
#include <array>
#include <iostream>  
#include <chrono> // Include the chrono library
#include "hborWithBhepv.hpp"
using namespace std;


// Function to process each query and calculate the statistics
// void processQueries(const std::string& mapName, int nPar) {
    
//     int queryCount = 5;
//     std::string queryFileName = "../Queries/" + mapName + "-queries";
//     std::ifstream queryFile(queryFileName);
//     if (!queryFile) {
//         std::cerr << "Failed to open query file: " << queryFileName << std::endl;
//         return;
//     }
    
//     auto startTimeRead = std::chrono::high_resolution_clock::now();
//     HBORBasic bhepv = HBORBasic(mapName, nPar);
//     bhepv.load();
//     auto endTimeRead = std::chrono::high_resolution_clock::now();
//     auto durationRead = std::chrono::duration_cast<std::chrono::milliseconds>(endTimeRead - startTimeRead);
//     std::cout<<"Read BHPV time:" << durationRead.count() << " milliseconds" << std::endl;

  
    
//     std::vector<double> hborQueryTimes; // To store the times for hbor method
//     std::vector<double> boaQueryTimes; // To store the times for boaPathRetrieval method
    
//     std::string line;
//     int queryID = 0;
//     while (std::getline(queryFile, line) && (queryCount <= 0 || queryID < queryCount)) {

//         int startNode, endNode;
//         std::istringstream iss(line);
//         if (!(iss >> startNode >> endNode)) {
//             std::cerr << "Invalid query format: " << line << std::endl;
//             continue;
//         }
//         cout<< "sNode: " <<startNode << ", dNode: " << endNode<<endl;
        
//         // Perform the boaPathRetrieval query and measure the time
//         auto startBoa = std::chrono::high_resolution_clock::now();
//         int boaNsolutions = bhepv.boa(startNode, endNode); 
//         auto endBoa = std::chrono::high_resolution_clock::now();
//         std::chrono::duration<double> boaDuration = endBoa - startBoa;
//         double boaQueryTime = boaDuration.count();
//         boaQueryTimes.push_back(boaQueryTime);
        
//         // Perform the hbor query and measure the time
//         auto startHbor = std::chrono::high_resolution_clock::now();
//         int hborNsolutions = 0;
//         hborNsolutions = bhepv.hbor(startNode, endNode); 
//         auto endHbor = std::chrono::high_resolution_clock::now();
//         std::chrono::duration<double> hborDuration = endHbor - startHbor;
//         double hborQueryTime = hborDuration.count();
//         hborQueryTimes.push_back(hborQueryTime);
        
        
//         std::cout << "Query (" << startNode << ", " << endNode << ") - HBOR: " << hborNsolutions << " solutions, BOA: " << boaNsolutions << " solutions." << std::endl;
//         std::cout << "Time used - HBOR: " << hborQueryTime << " seconds, BOA: " << boaQueryTime << " seconds." << std::endl;
        
//         queryID++;
        
//         hepv.cleanupGraphDataVector();
//     }
    
    
    
//     // Calculate statistics for HBOR method
//     double hborTotalRuntime = 0.0;
//     double hborMinTime = std::numeric_limits<double>::max();
//     double hborMaxTime = 0.0;
//     size_t hborNumQueries = hborQueryTimes.size();
    
//     std::sort(hborQueryTimes.begin(), hborQueryTimes.end());
//     double hborMedianTime = (hborNumQueries % 2 == 0) ? (hborQueryTimes[hborNumQueries / 2 - 1] + hborQueryTimes[hborNumQueries / 2]) / 2.0 : hborQueryTimes[hborNumQueries / 2];
    
//     for (double hborQueryTime : hborQueryTimes) {
//         hborTotalRuntime += hborQueryTime;
//         hborMinTime = std::min(hborMinTime, hborQueryTime);
//         hborMaxTime = std::max(hborMaxTime, hborQueryTime);
//     }
    
//     double hborAvgTime = (hborNumQueries > 0) ? hborTotalRuntime / hborNumQueries : 0.0;
    
//     // Calculate statistics for BOA method
//     double boaTotalRuntime = 0.0;
//     double boaMinTime = std::numeric_limits<double>::max();
//     double boaMaxTime = 0.0;
//     size_t boaNumQueries = boaQueryTimes.size();
    
//     std::sort(boaQueryTimes.begin(), boaQueryTimes.end());
//     double boaMedianTime = (boaNumQueries % 2 == 0) ? (boaQueryTimes[boaNumQueries / 2 - 1] + boaQueryTimes[boaNumQueries / 2]) / 2.0 : boaQueryTimes[boaNumQueries / 2];
    
//     for (double boaQueryTime : boaQueryTimes) {
//         boaTotalRuntime += boaQueryTime;
//         boaMinTime = std::min(boaMinTime, boaQueryTime);
//         boaMaxTime = std::max(boaMaxTime, boaQueryTime);
//     }
    
//     double boaAvgTime = (boaNumQueries > 0) ? boaTotalRuntime / boaNumQueries : 0.0;
    
//     // Print statistics for HBOR method
//     std::cout << "HBOR Method Statistics:" << std::endl;
//     std::cout << "Total Queries: " << hborNumQueries << std::endl;
//     std::cout << "Total Runtime: " << hborTotalRuntime << " seconds" << std::endl;
//     std::cout << "Average Time per Query: " << hborAvgTime << " seconds" << std::endl;
//     std::cout << "Min Time: " << hborMinTime << " seconds" << std::endl;
//     std::cout << "Max Time: " << hborMaxTime << " seconds" << std::endl;
//     std::cout << "Median Time: " << hborMedianTime << " seconds" << std::endl;
    
//     // Print statistics for BOA method
//     std::cout << "\nBOA Method Statistics:" << std::endl;
//     std::cout << "Total Queries: " << boaNumQueries << std::endl;
//     std::cout << "Total Runtime: " << boaTotalRuntime << " seconds" << std::endl;
//     std::cout << "Average Time per Query: " << boaAvgTime << " seconds" << std::endl;
//     std::cout << "Min Time: " << boaMinTime << " seconds" << std::endl;
//     std::cout << "Max Time: " << boaMaxTime << " seconds" << std::endl;
//     std::cout << "Median Time: " << boaMedianTime << " seconds" << std::endl;
// }


int testQueries(const std::string& mapName, int nPar) {
    HBORBasic bhepvR = HBORBasic(mapName, nPar);
    bhepvR.load();
    cout<< bhepvR.hbor(3,5) << endl;
    cout<< "boa: " << bhepvR.boa(3, 5) << endl; 
    cout<< bhepvR.hbor(8,1) << endl;
    cout<< "boa: " << bhepvR.boa(8, 1) << endl; 
    cout<< bhepvR.hbor(1,4) << endl;
    cout<< bhepvR.hbor(4,5) << endl;
    cout<< bhepvR.hbor(3,7) << endl;
    bhepvR.freeGraphDataVector();
    return 0;
}


int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <map_file> <nPartitions>" << std::endl;
        return 1;
    }
    std::string mapName = argv[1];
    int nPartitions = std::stoi(argv[2]);
//     processQueries(mapName, nPartitions);
    testQueries(mapName, nPartitions);
    return 0;
}






