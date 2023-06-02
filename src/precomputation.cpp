#include <cassert>
#include <vector>
#include <list>
#include <array>
#include <iostream>  
#include <chrono> // Include the chrono library
#include "hbor.h"
#include "biobjectiveGraph.h"
using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << "<mapName>" << std::endl;
        return 1;
    }
    auto startTimeTotal = std::chrono::high_resolution_clock::now();
    std::string mapName = argv[1];
    precomputation(mapName);
    auto endTimeTotal = std::chrono::high_resolution_clock::now();
    auto durationTotal = std::chrono::duration_cast<std::chrono::milliseconds>(endTimeTotal - startTimeTotal);
    std::cout << "Elapsed time of precomputation after graph partitioning: " << durationTotal.count() << " milliseconds" << std::endl;
    // Perform final processing
    std::cout << "Final processing executed" << std::endl;

    return 0;
}