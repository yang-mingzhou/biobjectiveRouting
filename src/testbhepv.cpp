#include <cassert>
#include <vector>
#include <list>
#include <array>
#include <iostream>  
#include <chrono> // Include the chrono library
#include "bhepv.hpp"
using namespace std;


int main() {

 
    BHEPV bhepv = BHEPV("test", 3);
    bhepv.PrecomputationAndSave();
    
    return 0;
}