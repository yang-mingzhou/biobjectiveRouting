#include "dijkstraForBoundaryEPV.hpp"


Bi_objective_Search::Bi_objective_Search(const string& foldername) : folderpath(foldername) {
    vector< vector<int> > mapData = fileToVector(folderpath + "/adjacent_LUBs.txt");
    adjNodes = createAdjNodes(mapData);

    vector< vector<int> > existings = fileToVector(folderpath + "/existingLUB.txt");
    existingLUBs = createAdjNodes(existings);

    for (auto const &pair: adjNodes) {
        key_nodes.push_back(pair.first);
    }
    sort(key_nodes.begin(), key_nodes.end());
    
    
    for (int i = 0; i < key_nodes.size(); i++) {
        nodeToIndex[key_nodes[i]] = i;
    }


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
}

// Bi_objective_Search::~Bi_objective_Search() {
// }


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




std::vector<std::vector<BoundaryPath>> Bi_objective_Search::DijkstraTraverse(int start) {
    // Use the HeapManager for the nodes to visit
    HeapManager heapManager;
    // Initial costs and start node, use the third heap which minimizes the (lb1, lb2)
    heapManager.pushToHeap(BoundaryPath({0, 0, 0, 0}, {start}), 3); 

    // Initialize the storage for paths
    std::vector<std::vector<BoundaryPath>> pathsByEndNode(key_nodes.size());

    while (!heapManager.isEmpty(3)) {
        BoundaryPath current = heapManager.popFromHeap(3);

        if (current.endNode() != start) {
            int index = nodeToIndex[current.endNode()]; 
            bool isDominated = false;
            for (const BoundaryPath& existingPath : pathsByEndNode[index]) {
                if (current.isDominatedBy(existingPath)) {
                    isDominated = true;
                    break;
                }
            }

            if (!isDominated) {
                pathsByEndNode[index].push_back(current);
            }
            else {
                continue;
            }
        }

        for (auto& neighbour : adjNodes[current.endNode()]) {
            // Avoid nodes if previously visited
            
            
            if(std::find(current.path.begin(), current.path.end(), neighbour.first) != current.path.end()) {
                continue;
            }
            
            // Check the logic before inserting into heap
            if (current.path.size() >= 2) {  // At least two nodes in the path
                int b = current.endNode();
                int a = current.path[current.path.size() - 2];  // Second last node in the path
                // no consecutive same partition
                if (nodePartitionMap[b] == nodePartitionMap[neighbour.first] && 
                    nodePartitionMap[a] == nodePartitionMap[b]) {
                    continue;
                }
            } else if (current.path.size() == 1 && 
                       nodePartitionMap[current.endNode()] == nodePartitionMap[neighbour.first]) {  
                // Special case for first two nodes in the path
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
            
            // if the new LUB is dominated by the existing LUBs, skip it.
            int c2e_ccost1 = newLUB[0];
            int c2e_ccost2 = newLUB[1];         
            int c2e_excost1 = existingLUBs.at(start).at(neighbour.first)[0];
            int c2e_excost2 = existingLUBs.at(start).at(neighbour.first)[1];
            int c2e_excost3 = existingLUBs.at(start).at(neighbour.first)[2];
            int c2e_excost4 = existingLUBs.at(start).at(neighbour.first)[3];
            if (c2e_ccost1 >= c2e_excost1 && c2e_ccost2 >= c2e_excost4) {
                if (!(c2e_ccost1 == c2e_excost1 && c2e_ccost2 == c2e_excost4)) {
                    continue;
                }
            }
            if (c2e_ccost2 >= c2e_excost2 && c2e_ccost1 >= c2e_excost3) {
                if (!(c2e_ccost2 == c2e_excost2 && c2e_ccost1 == c2e_excost3)) {
                    continue;
                }
            }
                 
            // generate new path
            std::vector<int> newPath = current.path;
            newPath.push_back(neighbour.first);
            BoundaryPath newCosts(newLUB, newPath);
            int index = nodeToIndex[neighbour.first];
            
            // if the new path is not dominated by any existing boundary paths.
            if (pathsByEndNode[index].empty() || 
                !newCosts.isDominatedBy(pathsByEndNode[index].front())) {
                heapManager.pushToHeap(newCosts, 3);
            }
        }
    }

    return pathsByEndNode;
}






bool Bi_objective_Search::dominates(const vector<int>& a, const vector<int>& b) {
    for (size_t i = 0; i < 4; ++i) {
        if (a[i] > b[i]) {
            return false;
        }
    }
    return true;
}


void Bi_objective_Search::allPairs() {
    int cnt = 0;
    
    for (int start : key_nodes) {
        // DijkstraTraverse now returns vector of vector of BoundaryPath
        std::vector<std::vector<BoundaryPath>> pathsGroups = DijkstraTraverse(start);

        // Process each group of paths and convert them to the vector<int> form
        for (const std::vector<BoundaryPath>& group : pathsGroups) {
            for (const BoundaryPath& bp : group) {
                allpath.push_back(bp.toVector());
            }
        }
        
        // Clear and release memory of pathsGroups
        pathsGroups.clear();
        std::vector<std::vector<BoundaryPath>>().swap(pathsGroups);
        
        if (cnt%100 ==0){
            cout << "done " << cnt << endl;
        }
        cnt ++;
    }
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