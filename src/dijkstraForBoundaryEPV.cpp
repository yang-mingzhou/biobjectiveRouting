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

    // If there are more attributes that need to be initialized, add them below:
    // Example:
    // someAttribute = someDefaultValue;
}

Bi_objective_Search::~Bi_objective_Search() {
    // Cleanup code goes here
    // Example:
    // delete somePointer; 
    // close someFileHandle;
}


vector<vector<int>> Bi_objective_Search::DijkstraTraverse(int start) {
    // Use a priority queue (min heap) for the nodes to visit
    priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> toVisit;
    toVisit.push({0, 0, 0, 0, start}); // Initial costs and start node

    // Stores paths that are not dominated
    vector<vector<int>> paths;

    while (!toVisit.empty()) {
        vector<int> current = toVisit.top();
        toVisit.pop();

        int currentNode = current[4];

        if (currentNode != start) {
            paths.push_back(current);
        }

        for (auto& neighbour : adjNodes[currentNode]) {
            vector<int> newCosts = {current[0] + neighbour.second[0], 
                                    current[1] + neighbour.second[1],
                                    current[2] + neighbour.second[2], 
                                    current[3] + neighbour.second[3], 
                                    neighbour.first};

            bool isDominated = false;
            for (auto& path : paths) {
                if (dominates(path, newCosts)) {
                    isDominated = true;
                    break;
                }
            }

            if (!isDominated) {
                toVisit.push(newCosts);
            }
        }
    }

    return paths;
}


bool Bi_objective_Search::dominates(const vector<int>& a, const vector<int>& b) {
    for (size_t i = 0; i < 4; ++i) {
        if (a[i] > b[i]) {
            return false;
        }
    }
    return true;
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

void Bi_objective_Search::allPairs() {
    for (int start : key_nodes) {
        vector<vector<int>> paths = DijkstraTraverse(start);
        // Add the paths to `allpath` or process them as needed
        allpath.insert(allpath.end(), paths.begin(), paths.end());
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