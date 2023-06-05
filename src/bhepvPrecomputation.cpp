#include <cassert>
#include <vector>
#include <list>
#include <array>
#include <iostream>  
#include <chrono> // Include the chrono library
#include "bhepv.hpp"
using namespace std;


int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << "<mapName> <nPartitions>" << std::endl;
        return 1;
    }
    auto startTimeTotal = std::chrono::high_resolution_clock::now();
    std::string mapName = argv[1];
    int nPartitions = std::stoi(argv[2]);
    BHEPV bhepv = BHEPV(mapName,nPartitions);
    bhepv.PrecomputationAndSave();
    bhepv.freeGraphDataVector();
    auto endTimeTotal = std::chrono::high_resolution_clock::now();
    auto durationTotal = std::chrono::duration_cast<std::chrono::milliseconds>(endTimeTotal - startTimeTotal);
    std::cout << "Elapsed time of precomputation after graph partitioning: " << durationTotal.count() << " milliseconds" << std::endl;
    // Perform final processing
    std::cout << "Precomputation executed" << std::endl;

    return 0;
}