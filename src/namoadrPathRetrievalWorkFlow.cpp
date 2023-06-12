#include <cassert>
#include <vector>
#include <list>
#include <array>
#include <iostream>  
#include <chrono> // Include the chrono library
#include <sys/resource.h>
#include "bhepvForNamoadr.hpp"
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
    HBORBasic bhepv = HBORBasic(mapName, nPar);
    long startMemory = getMemoryUsage();
    bhepv.load();
    long endMemory = getMemoryUsage();
    auto endTimeRead = std::chrono::high_resolution_clock::now();
    auto durationRead = std::chrono::duration_cast<std::chrono::milliseconds>(endTimeRead - startTimeRead);
    std::cout<<"Read BHPV time:" << durationRead.count() << " milliseconds" << std::endl;
    std::cout << "BHEPV memory usage: " << endMemory-startMemory << "KB.\n";
    

    
    std::string line;
    int queryID = 0;
    
    
    std::vector<double> namorQueryTimes; // To store the times for namor method
    std::vector<long> namorMemoryUsages; // To store the memory usages for namor method
    double namorQueryTimeSum = 0.0;
    
    int namorTotalSolutions = 0; // To store total solutions for namor method
    
    
    while (std::getline(queryFile, line) && (queryCount <= 0 || queryID < queryCount)) {

        int startNode, endNode;
        std::istringstream iss(line);
        if (!(iss >> startNode >> endNode)) {
            std::cerr << "Invalid query format: " << line << std::endl;
            continue;
        }
        cout<< "sNode: " <<startNode << ", dNode: " << endNode<<endl;
        


        
        
        // Perform the namor query and measure the time
        startMemory = getMemoryUsage();
        auto startNamor = std::chrono::high_resolution_clock::now();
        int namorNsolutions = bhepv.namor(startNode, endNode); 
        auto endNamor = std::chrono::high_resolution_clock::now();
        endMemory = getMemoryUsage();
        namorMemoryUsages.push_back(endMemory - startMemory);
        
        std::chrono::duration<double> namorDuration = endNamor - startNamor;
        double namorQueryTime = namorDuration.count();
        namorQueryTimes.push_back(namorQueryTime);

        

        std::cout << "Query (" << startNode << ", " << endNode << ") -  NAMOR: " << namorNsolutions << " solutions." << std::endl;
        std::cout << "Time used -  NAMOR: " << namorQueryTime*1000 << " milliseconds." << std::endl;
        
        queryID++;
        
        
//         std::cout << "Query (" << startNode << ", " << endNode << ") - HBOR: " << hborNsolutions << " solutions, BOA: " << boaNsolutions << " solutions." << std::endl;
//         std::cout << "Time used - HBOR: " << hborQueryTime*1000 << " milliseconds, BOA: " << boaQueryTime*1000 << " milliseconds." << std::endl;
        
//         queryID++;

        namorQueryTimeSum += namorQueryTime;
        namorTotalSolutions += namorNsolutions; // Accumulate solutions
    }
    
    bhepv.freeGraphDataVector();
    std::cout << "NAMOADR Query Times (in ms):" << std::endl;
    for (const auto& time : namorQueryTimes) {
        std::cout << time*1000 << " ";
    }
    std::cout << std::endl;
    // Calculate the average times and print them for namor method
    double namorAverageTime = namorQueryTimes.size() > 0 ? namorQueryTimeSum / namorQueryTimes.size() : 0;
    long namorAverageMemoryUsage = std::accumulate(namorMemoryUsages.begin(), namorMemoryUsages.end(), 0L) / namorMemoryUsages.size();
    double namorAverageSolutions = namorTotalSolutions / static_cast<double>(queryID); // Calculate average solutions
    std::cout << "Average NAMOR number of solutions: " << namorAverageSolutions << "\n"; // Output average solutions
    std::cout << "Average NAMOR query time: " << namorAverageTime*1000 << " milliseconds.\n";
    std::cout << "Average NAMOR memory usage: " << namorAverageMemoryUsage << "KB.\n";
    

   
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






