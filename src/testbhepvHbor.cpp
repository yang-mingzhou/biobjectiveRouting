#include <cassert>
#include <vector>
#include <list>
#include <array>
#include <iostream>  
#include <chrono> // Include the chrono library
#include "hborWithBhepv.hpp"
using namespace std;


int main() {
    HBORBasic bhepv = HBORBasic("test", 3);
    bhepv.load();
    cout<< bhepv.hbor(3,5) << endl;
    cout<< bhepv.hbor(8,1) << endl;
    cout<< bhepv.hbor(1,4) << endl;
    cout<< bhepv.hbor(4,5) << endl;
    cout<< bhepv.hbor(3,7) << endl;
    return 0;
}
