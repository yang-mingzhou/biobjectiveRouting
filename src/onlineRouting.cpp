#include <cassert>
#include <vector>
#include <list>
#include <array>
#include <iostream>  
#include <chrono> // Include the chrono library
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "hbor.h"
#include "biobjectiveGraph.h"
using namespace std;


void printQueryTimes(const std::vector<double>& hborQueryTimes, const std::vector<double>& boaQueryTimes) {
    std::cout << "HBOR Query Times (in ms):" << std::endl;
    for (const auto& time : hborQueryTimes) {
        std::cout << time*1000 << " ";
    }
    std::cout << std::endl;

    std::cout << "BOA Query Times (in ms):" << std::endl;
    for (const auto& time : boaQueryTimes) {
        std::cout << time*1000 << " ";
    }
    std::cout << std::endl;
}



// Function to process each query and calculate the statistics
void processQueries(const std::string& mapName, int nPar) {
    
    int queryCount = -1;
    std::string queryFileName = "../Queries/" + mapName + "-queries";
    std::ifstream queryFile(queryFileName);
    if (!queryFile) {
        std::cerr << "Failed to open query file: " << queryFileName << std::endl;
        return;
    }
    
    auto startTimeRead = std::chrono::high_resolution_clock::now();
    B3HEPV hepv = B3HEPV(mapName, nPar);
    auto endTimeRead = std::chrono::high_resolution_clock::now();
    auto durationRead = std::chrono::duration_cast<std::chrono::milliseconds>(endTimeRead - startTimeRead);
    std::cout<<"Read B3HPV time:" << durationRead.count() << " milliseconds" << std::endl;
    
    std::vector<double> hborQueryTimes; // To store the times for hbor method
    std::vector<double> boaQueryTimes; // To store the times for boaPathRetrieval method


    
    std::string line;
    int queryID = 0;
    
    double hborQueryTimeSum = 0.0, boaQueryTimeSum = 0.0;
    
    
    
    while (std::getline(queryFile, line) && (queryCount <= 0 || queryID < queryCount)) {

        int startNode, endNode;
        std::istringstream iss(line);
        if (!(iss >> startNode >> endNode)) {
            std::cerr << "Invalid query format: " << line << std::endl;
            continue;
        }
        cout<< "sNode: " <<startNode << ", dNode: " << endNode<<endl;

        // Perform the boaPathRetrieval query and measure the time
        auto startBoa = std::chrono::high_resolution_clock::now();
        int boaNsolutions = hepv.boaPathRetrieval(startNode, endNode); 
        auto endBoa = std::chrono::high_resolution_clock::now();
   

        std::chrono::duration<double> boaDuration = endBoa - startBoa;
//         auto boaDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endBoa - startBoa);
        double boaQueryTime = boaDuration.count();
        boaQueryTimes.push_back(boaQueryTime);
        
        
        // Perform the hbor query and measure the time

        auto startHbor = std::chrono::high_resolution_clock::now();
        int hborNsolutions = 0;
        hborNsolutions = hepv.hbor(startNode, endNode); 
        auto endHbor = std::chrono::high_resolution_clock::now();

//         auto hborDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endHbor - startHbor);
        std::chrono::duration<double> hborDuration = endHbor - startHbor;
        double hborQueryTime = hborDuration.count();
        hborQueryTimes.push_back(hborQueryTime);
//         std::cout << "Query (" << startNode << ", " << endNode << ") - HBOR with B3HEPV: " << hborNsolutions << " solutions" << std::endl;
       std::cout << "Query (" << startNode << ", " << endNode << ") - HBOR with B3HEPV: " << hborNsolutions << " solutions, BOA: " << boaNsolutions << " solutions." << std::endl;
//         std::cout << "Time used - HBOR with B3HEPV: " << hborQueryTime << " seconds" << std::endl;
        std::cout << "Time used - HBOR  with B3HEPV: " << hborQueryTime*1000 << " milliseconds, BOA: " << boaQueryTime*1000 << " milliseconds." << std::endl;
        
        queryID++;
        
        boaQueryTimeSum += boaQueryTime;
        hborQueryTimeSum += hborQueryTime;
    }
    
    hepv.freeGraphDataVector();
//     hepv.cleanupGraphDataVector();
    printQueryTimes(hborQueryTimes, boaQueryTimes);
    // Calculate the average times and print them
    double hborAverageTime = hborQueryTimes.size() > 0 ? hborQueryTimeSum / hborQueryTimes.size() : 0;
    double boaAverageTime = boaQueryTimes.size() > 0 ? boaQueryTimeSum / boaQueryTimes.size() : 0;
    std::cout << "Average HBOR with B3HEPV query time: " << hborAverageTime*1000 << " milliseconds.\n";
    std::cout << "Average BOA query time: " << boaAverageTime*1000 << " milliseconds.\n";
    
    

}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <map_file> <nPartitions>" << std::endl;
        return 1;
    }
    std::string mapName = argv[1];
    int nPartitions = std::stoi(argv[2]);
    processQueries(mapName, nPartitions);
    return 0;
}


