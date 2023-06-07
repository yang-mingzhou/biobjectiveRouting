#include <cassert>
#include <vector>
#include <list>
#include <array>
#include <iostream>  
#include <chrono> // Include the chrono library
#include "hborWithBhepv.hpp"
using namespace std;


int main() {
    HBORBasic bhepv = HBORBasic("BAY0", 20);
    bhepv.load();
    cout<< bhepv.hbor(85217, 110419) << endl;
    auto startBoa = std::chrono::high_resolution_clock::now();
    cout<< bhepv.hbor(50362, 135758) << endl;
    auto endBoa = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> boaDuration = endBoa - startBoa;
    double boaQueryTime = boaDuration.count();
    cout<<"time: "<<boaQueryTime<<endl;
    cout<< bhepv.hbor(155475, 6885) << endl;
    return 0;
}
