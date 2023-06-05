#include <cassert>
#include <vector>
#include <list>
#include <array>
#include <iostream>  
#include <chrono> // Include the chrono library
#include "json.hpp"
#include "boaPathRetrieval.hpp"
using namespace std;
using json = nlohmann::json;

// module load gcc/11.3.0
// gcc -o test.o test.cpp boaPathRetrieval.cpp heap.cpp boastar.cpp graph.cpp -lstdc++
// ./test.o

void test_paretoPathsInFragmentChar(){
      // convert filename to char *
    int nsolutions = 0;
    string filename = "../test/multigraph-road-d.txt";
    const char* charFilename = filename.c_str();
    unsigned (*solutions)[2] = paretoPathsInFragmentChar(1, 8, charFilename);
    // Access and print the values
    int i = 0;
    while (solutions[i][0] > 0) {
        cout<< "c1: "<< solutions[i][0] << ", c2: "<< solutions[i][1] << endl;
        nsolutions+=1;
        i++;
//     }   
    return nsolutions;
}



int main() {
    test_paretoPathsInFragmentChar();
    return 0;
}
