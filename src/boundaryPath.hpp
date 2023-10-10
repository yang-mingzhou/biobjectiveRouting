#ifndef BOUNDARYPATH_H
#define BOUNDARYPATH_H

#include <vector>
#include <queue>
#include <limits>
#include <tuple>
#include <string>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <queue>
#include <functional>
#include <unordered_set>
#include <iomanip>

class BoundaryPath {
    // for each boundaryPath, the lub: four elements are (LB1, LB2, UB1, UB2); path: a sequence of boundary nodes
    public:
    // Constructor
    BoundaryPath() = default;
    BoundaryPath(const std::vector<int>& lub_values, const std::vector<int>& path_values)
        : lub(lub_values), path(path_values) {
        // Ensure the lubs vector has exactly 4 elements
        if (lub_values.size() != 4) {
            throw std::runtime_error("Invalid number of lubs values. Expected 4 elements.");
        }
    }

    int startNode() const;
    int endNode() const;
    int getNode(int i) const;
    int len() const {
        return path.size();
    }
    bool isDominatedBy(const BoundaryPath& other) const;
    bool isLowerBoundDominatedBy(const BoundaryPath& other) const;
    void printPath() const;
    void printLub() const;
    bool isPreceeding(const BoundaryPath& other) const;
    BoundaryPath concatWith(const BoundaryPath& other) const;
    BoundaryPath reverse() const {
        std::vector<int> reversed_path(path.rbegin(), path.rend());
        return BoundaryPath(lub, reversed_path);
    }
    
    bool eq(const BoundaryPath& other) const {
        return path == other.path;
    }
    
    std::vector<int> toVector() const {
        // Initialize the vector with the lub values
        std::vector<int> result = lub;

        // Add the startNode and endNode
        result.push_back(startNode());
        result.push_back(endNode());

        // Append the path values
        result.insert(result.end(), path.begin(), path.end());

        return result;
    }

    std::vector<int> lub; // Contains 4 elements
    std::vector<int> path;

};

// Comparison functor for the min-heap.
struct BoundaryPathCompare {
    bool operator()(const BoundaryPath& a, const BoundaryPath& b) const {
        if (a.lub[2] == b.lub[2]) {
            return a.lub[3] > b.lub[3];  // Note the change to '>' to make it a min-heap.
        }
        return a.lub[2] > b.lub[2];
    }
};

struct BoundaryPathCompareSencondComp {
    bool operator()(const BoundaryPath& a, const BoundaryPath& b) const {
        if (a.lub[3] == b.lub[3]) {
            return a.lub[2] > b.lub[2];  // Note the change to '>' to make it a min-heap.
        }
        return a.lub[3] > b.lub[3];
    }
};

struct BoundaryPathCompareLowerFirstComp {
    bool operator()(const BoundaryPath& a, const BoundaryPath& b) const {
        if (a.lub[0] == b.lub[0]) {
            return a.lub[1] > b.lub[1];  // Note the change to '>' to make it a min-heap.
        }
        return a.lub[0] > b.lub[0];
    }
};

struct BoundaryPathCompareLowerSencondComp {
    bool operator()(const BoundaryPath& a, const BoundaryPath& b) const {
        if (a.lub[1] == b.lub[1]) {
            return a.lub[0] > b.lub[0];  // Note the change to '>' to make it a min-heap.
        }
        return a.lub[1] > b.lub[1];
    }
};


class HeapManager {
private:
    std::priority_queue<BoundaryPath, std::vector<BoundaryPath>, BoundaryPathCompareSencondComp> heapFirstComp;
    std::priority_queue<BoundaryPath, std::vector<BoundaryPath>, BoundaryPathCompare> heapSecondComp;
    std::priority_queue<BoundaryPath, std::vector<BoundaryPath>, BoundaryPathCompareLowerFirstComp> heapThirdComp;
    std::priority_queue<BoundaryPath, std::vector<BoundaryPath>, BoundaryPathCompareLowerSencondComp> heapFourthComp;

public:
    bool isDominatedBySmallest(const BoundaryPath& path, int comp) {
        switch (comp) {
            case 1:
                if (heapFirstComp.empty()) return false;
                return path.isDominatedBy(heapFirstComp.top());
            case 2:
                if (heapSecondComp.empty()) return false;
                return path.isDominatedBy(heapSecondComp.top());
            case 3:
                if (heapThirdComp.empty()) return false;
                return path.isDominatedBy(heapThirdComp.top());
            case 4:
                if (heapFourthComp.empty()) return false;
                return path.isDominatedBy(heapFourthComp.top());
            default:
                throw std::invalid_argument("Invalid component number");
        }
    }

    bool isEmpty(int comp) {
        switch (comp) {
            case 1: return heapFirstComp.empty();
            case 2: return heapSecondComp.empty();
            case 3: return heapThirdComp.empty();
            case 4: return heapFourthComp.empty();
            default: throw std::invalid_argument("Invalid component number");
        }
    }

    BoundaryPath popFromHeap(int comp) {
        BoundaryPath top;
        switch (comp) {
            case 1: 
                top = heapFirstComp.top(); 
                heapFirstComp.pop();
                break;
            case 2: 
                top = heapSecondComp.top(); 
                heapSecondComp.pop();
                break;
            case 3: 
                top = heapThirdComp.top(); 
                heapThirdComp.pop();
                break;
            case 4: 
                top = heapFourthComp.top(); 
                heapFourthComp.pop();
                break;
            default: 
                throw std::invalid_argument("Invalid component number");
        }
        return top;
    }

    void pushToHeap(const BoundaryPath& path, int comp) {
        switch (comp) {
            case 1: heapFirstComp.push(path); break;
            case 2: heapSecondComp.push(path); break;
            case 3: heapThirdComp.push(path); break;
            case 4: heapFourthComp.push(path); break;
            default: throw std::invalid_argument("Invalid component number");
        }
    }
};

#endif 