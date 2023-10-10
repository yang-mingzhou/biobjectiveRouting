#include "boundaryPath.hpp"

#include <string>
#include <sstream>
#include <climits>
#include <list>

using namespace std;


bool BoundaryPath::isDominatedBy(const BoundaryPath& other) const {
    // Compare the elements of lub
    if (eq(other)) {
        return true;
    }

    if ((lub[0] >= other.lub[0] && lub[1] >= other.lub[3])|| (lub[0] >= other.lub[2] && lub[1] >= other.lub[1])) {
        if ( (lub[0] != lub[2])|| (lub[1] != lub[3])|| (other.lub[0] != other.lub[2])|| (other.lub[1] != other.lub[3]) || !(lub[0] == other.lub[2] && lub[1] == other.lub[3]) ) {
            return true;
        }
    }
    return false;
}

bool BoundaryPath::isLowerBoundDominatedBy(const BoundaryPath& other) const {
    // Compare the elements of lub
    if (eq(other)) {
        return true;
    }

    if (lub[0] >= other.lub[0] && lub[1]>= other.lub[1]) {
            return true;
        }
  
    return false;
}

void BoundaryPath::printPath() const {
    cout << "Path: ";
    for (const int& node : path) {
        cout << node << ", ";
    }
    cout << endl;
}

void BoundaryPath::printLub() const {
    cout << "LUB: ";
    for (const int& i : lub) {
        cout << i << ", ";
    }
    cout << endl;
}

bool BoundaryPath::isPreceeding(const BoundaryPath& other) const {
    if (!path.empty() && !other.path.empty()) {
        return path.back() == other.path.front();
    }
    return false;
}

int BoundaryPath::startNode() const {
    if (!path.empty()) {
        return path.front();
    } else {
        // Handle the case where the path is empty
        return -1;
    }
}

int BoundaryPath::endNode() const {
    if (!path.empty()) {
        return path.back();
    } else {
        // Handle the case where the path is empty
        return -1;
    }
}

int BoundaryPath::getNode(int i) const {
    if (i >= 0 && i < path.size()) {
        return path[i];
    } else {
        // Handle the case where the index is out of range
        return -1;
    }
}

BoundaryPath BoundaryPath::concatWith(const BoundaryPath& other) const {
    if (isPreceeding(other)) {
        vector<int> newLub(4);
        vector<int> newPath;

        int maxInt = std::numeric_limits<int>::max();

        // Calculate the sum of corresponding lub values
        for (int i = 0; i < 4; ++i) {
            // Check for potential overflow
            if (lub[i] >= maxInt || other.lub[i] >= maxInt || (lub[i] > 0 && other.lub[i] > maxInt - lub[i])) {
                newLub[i] = maxInt; // If overflow would occur, set to max value
            } else {
                newLub[i] = lub[i] + other.lub[i];
            }
        }

        // Append the path elements from both paths, removing duplicates
        newPath = path;
        newPath.insert(newPath.end(), other.path.begin() + 1, other.path.end());

        return BoundaryPath(newLub, newPath);
    }
    throw runtime_error("Cannot concatenate paths. The paths are not preceeding each other.");
}