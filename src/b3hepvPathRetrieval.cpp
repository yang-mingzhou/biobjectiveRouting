#include <cassert>
#include <vector>
#include <list>
#include <array>
#include <iostream>  
#include <chrono> // Include the chrono library
#include <sys/resource.h>
#include "hborWithB3hepv.hpp"
using namespace std;


void printQueryTimes(const std::vector<double>& hborQueryTimes, const std::vector<double>& boaQueryTimes) {
    std::cout << "HBOR-B Query Times (in ms):" << std::endl;
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

void printMemoryUsage()
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    std::cout << "Memory usage: " << usage.ru_maxrss << "KB" << std::endl;
}

long getMemoryUsage()
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;
}

void processQueries(const std::string& mapName, int nPar) {
    
    int queryCount = -1;
    std::string queryFileName = "../Queries/" + mapName + "-queries";
    std::ifstream queryFile(queryFileName);
    if (!queryFile) {
        std::cerr << "Failed to open query file: " << queryFileName << std::endl;
        return;
    }
    
    
    
    auto startTimeRead = std::chrono::high_resolution_clock::now();
    B3HBORBasic b3hepv = B3HBORBasic(mapName, nPar);
    long startMemory = getMemoryUsage();
    b3hepv.load();
    long endMemory = getMemoryUsage();
    auto endTimeRead = std::chrono::high_resolution_clock::now();
    auto durationRead = std::chrono::duration_cast<std::chrono::milliseconds>(endTimeRead - startTimeRead);
    std::cout<<"Read B3HPV time:" << durationRead.count() << " milliseconds" << std::endl;
    std::cout << "B3HEPV memory usage: " << endMemory-startMemory << "KB.\n";
    std::vector<double> hborQueryTimes; // To store the times for hbor method
    std::vector<double> boaQueryTimes; // To store the times for boaPathRetrieval method
    
    std::vector<long> hborMemoryUsages; // To store the memory usages for hbor method
    std::vector<long> boaMemoryUsages; // To store the memory usages for boaPathRetrieval method

    
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
        startMemory = getMemoryUsage();
        // Perform the boaPathRetrieval query and measure the time
        auto startBoa = std::chrono::high_resolution_clock::now();
        int boaNsolutions = b3hepv.boa(startNode, endNode); 
        auto endBoa = std::chrono::high_resolution_clock::now();
        endMemory = getMemoryUsage();
        boaMemoryUsages.push_back(endMemory - startMemory);        

        std::chrono::duration<double> boaDuration = endBoa - startBoa;
//         auto boaDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endBoa - startBoa);
        double boaQueryTime = boaDuration.count();
        boaQueryTimes.push_back(boaQueryTime);
        
        
        // Perform the hbor query and measure the time
        startMemory = getMemoryUsage();
        auto startHbor = std::chrono::high_resolution_clock::now();
        int hborNsolutions = 0;
        hborNsolutions = b3hepv.hbor(startNode, endNode); 
        auto endHbor = std::chrono::high_resolution_clock::now();
        
        endMemory = getMemoryUsage();
        hborMemoryUsages.push_back(endMemory - startMemory);
        
//         auto hborDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endHbor - startHbor);
        std::chrono::duration<double> hborDuration = endHbor - startHbor;
        double hborQueryTime = hborDuration.count();
        hborQueryTimes.push_back(hborQueryTime);
        
        
        std::cout << "Query (" << startNode << ", " << endNode << ") - HBOR-B: " << hborNsolutions << " solutions, BOA: " << boaNsolutions << " solutions." << std::endl;
        std::cout << "Time used - HBOR-B: " << hborQueryTime*1000 << " milliseconds, BOA: " << boaQueryTime*1000 << " milliseconds." << std::endl;
        
        queryID++;
        
        boaQueryTimeSum += boaQueryTime;
        hborQueryTimeSum += hborQueryTime;
    }
    
    b3hepv.freeGraphDataVector();
    printQueryTimes(hborQueryTimes, boaQueryTimes);
    // Calculate the average times and print them
    double hborAverageTime = hborQueryTimes.size() > 0 ? hborQueryTimeSum / hborQueryTimes.size() : 0;
    double boaAverageTime = boaQueryTimes.size() > 0 ? boaQueryTimeSum / boaQueryTimes.size() : 0;
    std::cout << "Average HBOR-B query time: " << hborAverageTime*1000 << " milliseconds.\n";
    std::cout << "Average BOA query time: " << boaAverageTime*1000 << " milliseconds.\n";
    
    
    // Compute average memory usages
    long hborAverageMemoryUsage = std::accumulate(hborMemoryUsages.begin(), hborMemoryUsages.end(), 0L) / hborMemoryUsages.size();
    long boaAverageMemoryUsage = std::accumulate(boaMemoryUsages.begin(), boaMemoryUsages.end(), 0L) / boaMemoryUsages.size();

    std::cout << "Average HBOR-B memory usage: " << hborAverageMemoryUsage << "KB.\n";
    std::cout << "Average BOA memory usage: " << boaAverageMemoryUsage << "KB.\n";
   
}


int testQueries(const std::string& mapName, int nPar) {
    B3HBORBasic bhepvR = B3HBORBasic(mapName, nPar);
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






