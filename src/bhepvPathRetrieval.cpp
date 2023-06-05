#include <cassert>
#include <vector>
#include <list>
#include <array>
#include <iostream>  
#include <chrono> // Include the chrono library
#include "hborWithBhepv.hpp"
using namespace std;


void printQueryTimes(const std::vector<double>& hborQueryTimes, const std::vector<double>& boaQueryTimes) {
    std::cout << "HBOR Query Times (in s):" << std::endl;
    for (const auto& time : hborQueryTimes) {
        std::cout << time << " ";
    }
    std::cout << std::endl;

    std::cout << "BOA Query Times (in s):" << std::endl;
    for (const auto& time : boaQueryTimes) {
        std::cout << time << " ";
    }
    std::cout << std::endl;
}

void processQueries(const std::string& mapName, int nPar) {
    
    int queryCount = 5;
    std::string queryFileName = "../Queries/" + mapName + "-queries";
    std::ifstream queryFile(queryFileName);
    if (!queryFile) {
        std::cerr << "Failed to open query file: " << queryFileName << std::endl;
        return;
    }
    
    auto startTimeRead = std::chrono::high_resolution_clock::now();
    HBORBasic bhepv = HBORBasic(mapName, nPar);
    bhepv.load();
    auto endTimeRead = std::chrono::high_resolution_clock::now();
    auto durationRead = std::chrono::duration_cast<std::chrono::milliseconds>(endTimeRead - startTimeRead);
    std::cout<<"Read BHPV time:" << durationRead.count() << " milliseconds" << std::endl;

  
    
    std::vector<double> hborQueryTimes; // To store the times for hbor method
    std::vector<double> boaQueryTimes; // To store the times for boaPathRetrieval method
    
    std::string line;
    int queryID = 0;
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
        int boaNsolutions = bhepv.boa(startNode, endNode); 
        auto endBoa = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> boaDuration = endBoa - startBoa;
        double boaQueryTime = boaDuration.count();
        boaQueryTimes.push_back(boaQueryTime);
        
        // Perform the hbor query and measure the time
        auto startHbor = std::chrono::high_resolution_clock::now();
        int hborNsolutions = 0;
        hborNsolutions = bhepv.hbor(startNode, endNode); 
        auto endHbor = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> hborDuration = endHbor - startHbor;
        double hborQueryTime = hborDuration.count();
        hborQueryTimes.push_back(hborQueryTime);
        
        
        std::cout << "Query (" << startNode << ", " << endNode << ") - HBOR: " << hborNsolutions << " solutions, BOA: " << boaNsolutions << " solutions." << std::endl;
        std::cout << "Time used - HBOR: " << hborQueryTime << " seconds, BOA: " << boaQueryTime << " seconds." << std::endl;
        
        queryID++;
        
        
    }
    
    bhepv.freeGraphDataVector();
    printQueryTimes(hborQueryTimes, boaQueryTimes);
   
}


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
    processQueries(mapName, nPartitions);
//     testQueries(mapName, nPartitions);
    return 0;
}






