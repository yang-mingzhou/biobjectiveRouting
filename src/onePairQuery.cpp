#include <cassert>
#include <vector>
#include <list>
#include <array>
#include <iostream>  
#include <chrono> // Include the chrono library
#include <fstream>
#include <sstream>
#include <algorithm>
#include "hbor.h"
#include "biobjectiveGraph.h"
using namespace std;

// gcc -O3 -std=c99 -o onePair.o onePairQuery.cpp hbor.cpp biobjectiveGraph.cpp pathRetrieval.c heap.c boastar.c graph.c -lstdc++

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " <map_file> <start_node> <end_node> <npartitions>" << std::endl;
        return 1;
    }

    std::string mapName = argv[1];
    int startNode = std::stoi(argv[2]);
    int endNode = std::stoi(argv[3]);
    int nPartitions = std::stoi(argv[4]);
    auto startTimeTotal = std::chrono::high_resolution_clock::now();
    B3HEPV hepv = B3HEPV(mapName, nPartitions);
    auto endTimeTotal = std::chrono::high_resolution_clock::now();
    auto durationTotal = std::chrono::duration_cast<std::chrono::milliseconds>(endTimeTotal - startTimeTotal);
    std::cout<<"Read B3HPV time:" << durationTotal.count() << " milliseconds" << std::endl;

    
    startTimeTotal = std::chrono::high_resolution_clock::now();
    int nBoa = hepv.boaPathRetrieval(startNode, endNode);
    endTimeTotal = std::chrono::high_resolution_clock::now();
    durationTotal = std::chrono::duration_cast<std::chrono::milliseconds>(endTimeTotal - startTimeTotal);
    std::cout<<"nSolutions in boa:" << nBoa << "BOA*: " << durationTotal.count() << " milliseconds" << std::endl;
    
    startTimeTotal = std::chrono::high_resolution_clock::now();
    int nsolutions = hepv.hbor(startNode,endNode);
    endTimeTotal = std::chrono::high_resolution_clock::now();
    durationTotal = std::chrono::duration_cast<std::chrono::milliseconds>(endTimeTotal - startTimeTotal);
    std::cout<< "nSolutions:" << nsolutions << ". Online routing time: " << durationTotal.count() << " milliseconds" << std::endl;
}