#include <iostream>
#include <vector>
#include <chrono>
#include "dijkstraForBoundaryEPV.hpp"

using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << "<mapName>" << std::endl;
        return 1;
    }
    auto startTimeTotal = std::chrono::high_resolution_clock::now();
    std::string mapName = argv[1];
    std::string foldername = "../b3hepv/"+mapName;
    Bi_objective_Search bodSearch(foldername);

    bodSearch.allPairs();

    bodSearch.save_all_path("json");

    bodSearch.convert_map_of_map_to_json_file();
    auto endTimeTotal = std::chrono::high_resolution_clock::now();
    auto durationTotal = std::chrono::duration_cast<std::chrono::milliseconds>(endTimeTotal - startTimeTotal);
    std::cout << "Elapsed time of encoding boundary path view : " << durationTotal.count() << " milliseconds" << std::endl;

    return 0;
}

