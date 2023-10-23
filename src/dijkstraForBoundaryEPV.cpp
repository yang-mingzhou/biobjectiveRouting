#include "dijkstraForBoundaryEPV.hpp"


Bi_objective_Search::Bi_objective_Search(const string& foldername) : folderpath(foldername) {
    vector< vector<int> > mapData = fileToVector(folderpath + "/adjacent_LUBs.txt");
    adjNodes = createAdjNodes(mapData);

    for (auto const &pair: adjNodes) {
        key_nodes.push_back(pair.first);
    }
    sort(key_nodes.begin(), key_nodes.end());
    
    
    for (int i = 0; i < key_nodes.size(); i++) {
        nodeToIndex[key_nodes[i]] = i;
    }
    
    vector< vector<int> > existings = fileToVector(folderpath + "/existingLUB.txt");
    existingLUBs = createAdjNodesVector(existings);
    
     // Initialize each dimension of preComputedPaths with empty vectors
    preComputedPaths = std::vector<std::vector<std::vector<BoundaryPath>>>(key_nodes.size(), std::vector<std::vector<BoundaryPath>>(key_nodes.size()));



    string filename = folderpath + "/boundaryNodePartition.json";
    std::ifstream file(filename);

    if (file.is_open()) {
        // Read the JSON from the file
        json root;
        file >> root;
        file.close();

        // Convert the JSON to boundary nodes and partitions
        for (json::iterator it = root.begin(); it != root.end(); ++it) {
            int node = std::stoi(it.key());
            int partition = it.value().get<int>();

            // Add the node and partition to the map
            nodePartitionMap[node] = partition;
        }

        std::cout << "Boundary nodes read from " << filename << std::endl;
    } else {
        std::cerr << "Failed to open file: " << filename << std::endl;
    }
    
    
    
     // load boundaryEncodedPathView from json
    std::string fileNameFragmentEPV = folderpath + "/fragmentEPV.json";
    std::ifstream i_fepv(fileNameFragmentEPV);
    json j_fepv;
    i_fepv >> j_fepv;
    int cnt_FragmentPath =0;
    for (json::iterator it = j_fepv.begin(); it != j_fepv.end(); ++it) {
            int outerKey = std::stoi(it.key());
            for (json::iterator inner_it = it.value().begin(); inner_it != it.value().end(); ++inner_it) {
                int innerKey = std::stoi(inner_it.key());
                fragmentEncodedPathView[outerKey][innerKey] = inner_it.value().get<std::vector<std::vector<int>>>();
                cnt_FragmentPath++;
            }
        }
        cout<< "Number of encoded fragment paths: " << cnt_FragmentPath << endl;

    }

Bi_objective_Search::~Bi_objective_Search() {
//     std::cout << "Destructor called for Bi_objective_Search" << std::endl;
}


vector<vector<int>> Bi_objective_Search::fileToVector(const string& filename) {
    ifstream fin(filename);
    vector<vector<int>> map_info;
    int s_node, d_node, cost1, cost2, cost3, cost4;

    while(fin >> s_node >> d_node >> cost1 >> cost2 >> cost3 >> cost4){
        map_info.push_back({s_node, d_node, cost1, cost2, cost3, cost4});
    }
    fin.close();

    return map_info;
}

unordered_map<int, unordered_map<int, vector<int>>> Bi_objective_Search::createAdjNodes(vector< vector<int> > mapData) {
    unordered_map<int, unordered_map<int, vector<int>>> adjNodes;
    for (const auto& edge : mapData) {
        adjNodes[edge[0]][edge[1]] = {edge[2], edge[3], edge[4], edge[5]};
    }
    return adjNodes;
}

vector<vector<vector<int>>> Bi_objective_Search::createAdjNodesVector(const vector<vector<int>>& mapData) {
    // Initialize with empty vectors given the size of key_nodes
    vector<vector<vector<int>>> adjNodesVector(key_nodes.size(), vector<vector<int>>(key_nodes.size()));

    for (const auto& edge : mapData) {
        int sourceIndex = nodeToIndex[edge[0]];
        int destIndex = nodeToIndex[edge[1]];
        adjNodesVector[sourceIndex][destIndex] = {edge[2], edge[3], edge[4], edge[5]};
    }

    return adjNodesVector;
}



void Bi_objective_Search::DijkstraTraverse(int start) {
    // Use the HeapManager for the nodes to visit
    HeapManager heapManager;
    // Initial costs and start node, use the third heap which minimizes the (lb1, lb2)
    BoundaryPath startPath = BoundaryPath({0, 0, 0, 0}, {start});
    startPath.setElbowPoint(0,0);
    
//     startPath.parent = nullptr;
//     startPath.hasParent = false;
    
    heapManager.pushToHeap(startPath, 3); 
    int startPartition = nodePartitionMap[start];
    int startIndex = nodeToIndex[start];  // Get the index of the start node
    int cnt_pruned = 0;
    // Seed the heap with reversed paths from preComputedPaths for nodes smaller than the current start node.
    for (int destIndex = 0; destIndex < startIndex; ++destIndex) {
        for (const BoundaryPath& bp : preComputedPaths[destIndex][startIndex]) {
            heapManager.pushToHeap(bp.reverse(), 3);  
        }
    }

   
    // since every possible paths that has smaller destination is already loaded, 
    // for current popped path: if the destnode is smaller start node, skip the dominance check and do expansion diectly
    // for the potential expended node: if the expended node is smaller start node, skip it
    while (!heapManager.isEmpty(3)) {
        BoundaryPath current = heapManager.popFromHeap(3);  
       
        if (current.endNode() > start) {
            
            int currentIndex = nodeToIndex[current.endNode()]; 
            bool isDominated = false;
            
            for (const BoundaryPath& existingPath : preComputedPaths[startIndex][currentIndex]) {                  
                if (current.isDominatedBy(existingPath)) {
                    isDominated = true;
                    break;
                }
                else if (current.isDominatedByElbowPointOf(existingPath)){
                    isDominated = true;
                    cnt_pruned ++;
                    break;
                }
            }

            if (!isDominated) {
                preComputedPaths[startIndex][currentIndex].push_back(current);
            }
            else {
                continue;
            }
        }
        for (auto& neighbour : adjNodes[current.endNode()]) {
            
            int expendNode = neighbour.first;

            // all paths that has smaller destination are already in the heap
            if(expendNode <= start){
                continue;
            }
            // Avoid nodes if previously visited
            if(std::find(current.path.begin(), current.path.end(), expendNode) != current.path.end()) {
                continue;
            }       
            
            // avoid int overflow
            bool willOverflow = false;
            for (int i = 0; i < 4; i++) {
                if (neighbour.second[i] > INT_MAX - current.lub[i]) {
                    willOverflow = true;
                    break;
                }
            }
            if (willOverflow) {              
                continue;
            }

            std::vector<int> newLUB = {
                current.lub[0] + neighbour.second[0], 
                current.lub[1] + neighbour.second[1],
                current.lub[2] + neighbour.second[2], 
                current.lub[3] + neighbour.second[3]
            };
            
            // generate new path
            std::vector<int> newPath = current.path;
            newPath.push_back(expendNode);
            BoundaryPath newCosts(newLUB, newPath);
//             BoundaryPath newCosts(newLUB, {expendNode});
//             newCosts.parent = &current;
//             newCosts.hasParent = true;
            
            newCosts.firstConsecutiveSamePartition = current.firstConsecutiveSamePartition;
            newCosts.updateTotalCostRecord(newLUB);  
            
            int currentDestPartition = nodePartitionMap[expendNode];
                       
            // Check if consective 3 nodes in the same fragment
            if (current.path.size() >= 2) {  // At least two nodes in the path
                int b = current.endNode();

                // Check for consecutive same partition at the end
                if (nodePartitionMap[b] == currentDestPartition) {
                    newCosts.lastConsecutiveSamePartition = true;
                }

                // Avoid adding paths with more than two consecutive nodes in the same partition at the end
                if (newCosts.lastConsecutiveSamePartition && current.lastConsecutiveSamePartition) {
                    continue;
                }
            }
            else {
                // For paths of size 1, check the firstConsecutiveSamePartition flag
                if (startPartition == currentDestPartition) {
                    newCosts.firstConsecutiveSamePartition = true;
                    newCosts.lastConsecutiveSamePartition = true;
                }
            }
            
            // set elbow points
            if (newCosts.lastConsecutiveSamePartition){           
                const auto& vec = fragmentEncodedPathView.at(current.endNode()).at(expendNode);                                    
                if (!vec.empty()){
                    int midIndex = vec.size() / 2;
                    newCosts.setElbowPoint(current.elbow_point.first+ vec[midIndex][0],current.elbow_point.second+ vec[midIndex][1]);
                }
                else {
                    newCosts.setElbowPoint(current.elbow_point.first+neighbour.second[0],current.elbow_point.second+neighbour.second[3]);
                }
            }
            else {
                newCosts.setElbowPoint(current.elbow_point.first+neighbour.second[0],current.elbow_point.second+neighbour.second[3]);
            }


            // check is dominated by existing lubs
            if (isDominated_for_all_nodes(newCosts)) {
                continue;
            }         
            
            int index = nodeToIndex[neighbour.first];    
                         
           // Check if the new path is dominated by any existing boundary paths.
            bool isDominated = false;
            for (const BoundaryPath& existingPath : preComputedPaths[startIndex][index]) {
                if (newCosts.isDominatedBy(existingPath)) {
                    isDominated = true;
                      break;
                }
                else if (newCosts.isDominatedByElbowPointOf(existingPath)){
                    isDominated = true;
                    cnt_pruned ++;
                    break;
                }
            }

            if (!isDominated) {
                // Insert new path into preComputedPaths and push to heap for exploration
                heapManager.pushToHeap(newCosts, 3);
            }
        }
    }
    
//     cout << "number of paths pruned by elbow point: " << cnt_pruned<< endl;
}

/* 
    Input: expanded node, BoundaryPath containing the new costs and path
    Output: boolean true or false
    Function: For each pair between expanded node and respective node on the path, check if it is dominated or not
*/
bool Bi_objective_Search::isDominated_for_all_nodes(const BoundaryPath& newCosts) {
    const auto& total_cost_record = newCosts.total_cost_record;
    const auto& path = newCosts.path;
    int start = path.front();
    int expandedNode = path.back();
    int startIndex = nodeToIndex[start];
    int expandedIndex = nodeToIndex[expandedNode];
    
    // Checking dominance for the complete path
    int pathCost1 = total_cost_record.back()[0];
    int pathCost2 = total_cost_record.back()[1];

    int entire_c2e_excost1 = existingLUBs[startIndex][expandedIndex][0];
    int entire_c2e_excost2 = existingLUBs[startIndex][expandedIndex][1];
    int entire_c2e_excost3 = existingLUBs[startIndex][expandedIndex][2];
    int entire_c2e_excost4 = existingLUBs[startIndex][expandedIndex][3];

    if ((pathCost1 >= entire_c2e_excost1 && pathCost2 >= entire_c2e_excost4 && !(pathCost1 == entire_c2e_excost1 && pathCost2 == entire_c2e_excost4)) 
        || (pathCost2 >= entire_c2e_excost2 && pathCost1 >= entire_c2e_excost3 && !(pathCost2 == entire_c2e_excost2 && pathCost1 == entire_c2e_excost3))) {
        return true;
    }
   
    for (int i = 0; i < total_cost_record.size()-1; i++) { // We skip the last element since it's the s2e_current_cost
        int subpathCost1 = total_cost_record.back()[0] - total_cost_record[i][0];
        int subpathCost2 = total_cost_record.back()[1] - total_cost_record[i][1];
        int nodeIndex = nodeToIndex[path[i]];
        int c2e_excost1 = existingLUBs[nodeIndex][expandedIndex][0];
        int c2e_excost2 = existingLUBs[nodeIndex][expandedIndex][1];
        int c2e_excost3 = existingLUBs[nodeIndex][expandedIndex][2];
        int c2e_excost4 = existingLUBs[nodeIndex][expandedIndex][3];

        if (subpathCost1 >= c2e_excost1 && subpathCost2 >= c2e_excost4) {
            if (!(subpathCost1 == c2e_excost1 && subpathCost2 == c2e_excost4)) {
                return true;
            }
        }
        if (subpathCost2 >= c2e_excost2 && subpathCost1 >= c2e_excost3) {
            if (!(subpathCost2 == c2e_excost2 && subpathCost1 == c2e_excost3)) {
                return true;
            }
        }
    }


    return false;
}

void Bi_objective_Search::allPairs() {
    int cnt = 0;
    cout << key_nodes.size() << endl;
    int pruned = 0;
    for (int start : key_nodes) {
        int startIndex = nodeToIndex[start];  // Get the index of the start node

        // Call DijkstraTraverse for the current start node
        DijkstraTraverse(start);

        // Process each group of paths in preComputedPaths and convert them to the vector<int> form
        for (int i = 0; i < preComputedPaths[startIndex].size(); i++) {
            // Skip if end node index is not higher than start node index
            if (i > startIndex) {
                for (const BoundaryPath& bp : preComputedPaths[startIndex][i]) {
                    if (!bp.firstConsecutiveSamePartition && !bp.lastConsecutiveSamePartition) {
                        allpath.push_back(bp.toVector());
                    } else {
                        pruned++;
                    }
                }
            }
            // Clear unneeded paths for memory optimization
            else {
                preComputedPaths[i][startIndex].clear();
                preComputedPaths[i][startIndex].shrink_to_fit();
            }
        }

        if (cnt % 100 == 0) {
            cout << "done " << cnt << endl;
        }
        cnt++;
    }
    cout << "pruned paths: " << pruned << endl;
}





/* 
input: filetype "txt" or "json"
output: None
function: save the result of DFS to the file with specified filetype 
*/
void Bi_objective_Search::save_all_path(string filetype) {
    if (filetype=="txt") {

        ofstream myfile;
        myfile.open(folderpath + "/boundaryEncodedPathView.txt");
        for (int i=0; i<allpath.size(); i++) {
            for (int j=0; j<allpath[i].size(); j++) {
                myfile << (allpath)[i][j] << " ";
            }
            myfile << "\n";
        }
        myfile.close();

    } else if (filetype=="json") {

        json s_node_key;
        for (int i=0; i<allpath.size(); i++){
            json d_node_key;
            string s_node = to_string(allpath[i][4]);
            string d_node = to_string(allpath[i][5]);
            allpath[i].erase(allpath[i].begin()+4, allpath[i].begin()+6);
            vector<vector<int>> temp;
            temp.push_back(allpath[i]);

            // if there is no s_node as a key in json 
            if (!s_node_key.contains(s_node)){
                d_node_key[d_node] = temp;
                s_node_key[s_node] = d_node_key;
            } else {
                // if there is a s_node as a key but no d_node as a key
                if (!s_node_key[s_node].contains(d_node)){
                    s_node_key[s_node][d_node] = temp;
                // if both s_node and d_noe exists as a key in json
                } else {
                    s_node_key[s_node][d_node].push_back(allpath[i]);
                }
            }
        }

        // ATTENTION: key in the json file would be all stirngs
        ofstream jsonfile(folderpath + "/boundaryEncodedPathView.json");
        jsonfile << s_node_key.dump();
        jsonfile.close();
    
    } else if (filetype == "compact") {

        ofstream myfile;
        myfile.open(folderpath + "/boundaryEncodedPathView.txt");
        for (int i=0; i<allpath.size(); i++) {
            for (int j=0; j<allpath[i].size(); j++) {
                if (j < 6 || j == 7) {
                    myfile << (allpath)[i][j] << " ";
                }
            }
            myfile << "\n";
        }
        myfile.close();


    }
    
}

void Bi_objective_Search::convert_map_of_map_to_json_file() {

    json s_node_key;
    for (auto it = adjNodes.begin(); it != adjNodes.end(); ++it) {

        unordered_map<int, vector<int>> cur_node_adj = adjNodes[it->first];
        json d_node_key;
        string s_node = to_string(it->first);

        for (auto it2 = cur_node_adj.begin(); it2 != cur_node_adj.end(); ++it2) {
            string d_node = to_string(it2->first);
            d_node_key[d_node].push_back(it2->second);
        }

        s_node_key[s_node] = d_node_key;
    }

    // ATTENTION: key in the json file would be all stirngs
    ofstream jsonfile(folderpath + "/adjacent_LUB.json");
    jsonfile << s_node_key.dump(4);
    jsonfile.close();
}