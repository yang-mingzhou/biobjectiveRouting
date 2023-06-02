#include <iostream>
#include <vector>
#include <fstream>
#include <list>
#include <unordered_map>
#include <algorithm>
#include <time.h>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class Bi_objective_DFS{

    public:
    unordered_map<int, unordered_map<int, vector<int>>> adjNodes, existingLUBs;
    vector<vector<int>> allpath;

    int current_s_node, current_d_node;
    int s2d_existing_cost3, s2d_existing_cost4;

    vector<int> key_nodes;
    string folderpath;
    std::unordered_map<int, int> nodePartitionMap;
    

    // initialization
    Bi_objective_DFS(const string& foldername) {

        folderpath = foldername;
        vector< vector<int> > mapData = fileToVector(folderpath + "/adjacent_LUBs.txt");
        vector< vector<int> > existings = fileToVector(folderpath + "/existingLUB.txt");
    
        adjNodes = createAdjNodes(mapData);
        existingLUBs = createAdjNodes(existings);

        for (auto const &pair: adjNodes) {
            key_nodes.push_back(pair.first);
        }
        sort(key_nodes.begin(), key_nodes.end());
        
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
        
//         for (const auto& entry : nodePartitionMap) {
//             std::cout << "Node: " << entry.first << ", Partition: " << entry.second << std::endl;
//         }


    }

    /*
    input: The name of the file to be read.
    output: A vector of vectors storing the content of the .txt file.

    function:
    Read the map information from a .txt file, return a vector of vectors of the content.The format is  “s_node d_node cost1 cost2 cost3 cost4” in each line of the file.
    */
    vector<vector<int>> fileToVector(const string& filename) {
        ifstream fin(filename);
        // variables
        vector<vector<int>> map_info;
        int s_node, d_node, cost1, cost2, cost3, cost4;

        if (fin.is_open()) {
            // load fin and store data into vector
            while(fin >> s_node >> d_node >> cost1 >> cost2 >> cost3 >> cost4){
                vector<int> line_info;

                line_info.push_back(s_node);
                line_info.push_back(d_node);
                line_info.push_back(cost1);
                line_info.push_back(cost2);
                line_info.push_back(cost3);
                line_info.push_back(cost4);

                map_info.push_back(line_info);
            }
            fin.close();
        } else {
            cout << "Unable to open the file." << endl;
            abort();
        }

        return map_info;
    }


    /*
    * input: edge data in the graph
    * output: index that organizes edge data
    *         key->s_node, value->[d_node, c1, c2, c3, c4]
    *
    * function: convert the given edge data to hash tables that would be used in DFS search
    */
    unordered_map<int, unordered_map<int, vector<int>>> createAdjNodes(vector< vector<int> > mapData)
    {
        unordered_map<int, unordered_map<int, vector<int>>> adjNodes;
        vector<int> dNode_cost;

        // iterate through all vectors
        for (int i=0; i<mapData.size(); i++)
        {
            // if the node already exists as a key
            if (adjNodes.count(mapData[i][0])){
                // add new edge data
                dNode_cost.assign(&mapData[i][2], &mapData[i][6]);
                adjNodes.at(mapData[i][0])[mapData[i][1]] = dNode_cost;
                //adjNodes.at(mapData[i][0]).push_back(dNode_cost);
            } else {
                // create new index and add edge data
                //vector<int> dNode_cost_list;
                unordered_map<int, vector<int>> dest2cost;
                dNode_cost.assign(&mapData[i][2], &mapData[i][6]);
                //dNode_cost_list.push_back(dNode_cost);

                dest2cost[mapData[i][1]] = dNode_cost;
                adjNodes[mapData[i][0]] = dest2cost;
            }
        }
        return adjNodes;
    }

    /* 
    input: expanded node, total cost record so far, current total cost from start to expanded node, current path
    output: boolean true or false
    function: for each pair between expanded node and respective node on the path, check if it is dominated or not
     */
    bool isDominated_for_all_nodes(int expanded_node, vector<vector<int>> total_cost_record, vector<int> s2e_current_cost, vector<int> path) {
        
        for (int i=0; i<total_cost_record.size(); i++) {
            int c2e_ccost1 = s2e_current_cost[0] - total_cost_record[i][0];
            int c2e_ccost2 = s2e_current_cost[1] - total_cost_record[i][1];
            
            int c2e_excost1 = existingLUBs.at(path[i]).at(expanded_node)[0];
            int c2e_excost2 = existingLUBs.at(path[i]).at(expanded_node)[1];
            int c2e_excost3 = existingLUBs.at(path[i]).at(expanded_node)[2];
            int c2e_excost4 = existingLUBs.at(path[i]).at(expanded_node)[3];


            if (c2e_ccost1 >= c2e_excost1 && c2e_ccost2 >= c2e_excost4) {
                if (!(c2e_ccost1 == c2e_excost1 && c2e_ccost2 == c2e_excost4)) {
                    return true;
                }
            }
            if (c2e_ccost2 >= c2e_excost2 && c2e_ccost1 >= c2e_excost3) {
                if (!(c2e_ccost2 == c2e_excost2 && c2e_ccost1 == c2e_excost3)) {
                    return true;
                }
            }
        }
        return false;
    }

    /*
    input: expanded node, current total cost from start to expanded node
    output: boolean true or false
    function: sum current total cost from start node to expanded node with the assumed cost from expanded to destination node
              Then check if it is dominated or not
     
    bool isDominated_for_a_star(int expanded_node, vector<int> s2e_current_cost) {

        int e2d_excost1 = existingLUBs.at(expanded_node).at(current_d_node)[0];
        int e2d_excost2 = existingLUBs.at(expanded_node).at(current_d_node)[1];
        if (s2e_current_cost[0]+e2d_excost1 >= s2d_existing_cost3 && s2e_current_cost[1]+e2d_excost2 >= s2d_existing_cost4) {
            if (!(s2e_current_cost[0]+e2d_excost1 == s2d_existing_cost3 && s2e_current_cost[1]+e2d_excost2 == s2d_existing_cost4)) {
                return true;
            }
        }
        return false;
    }*/


    /* 
    * input: current start_node, dest_node, current path from s_node, current total cost, edge index, LUBs, and vector of vector storing all path
    * 
    * function: recursive function to find all paths between specified nodes through DFS.
    */
    void DFS(int current_node, vector<int> path, vector<vector<int>> total_cost_record, vector<int> current_total_cost, vector<vector<int>> *all_paths, bool withInFragmentFlag)
    {
        path.push_back(current_node);
        total_cost_record.push_back(current_total_cost);
        
        /* cout << "path->";
        for (int i=0; i<path.size(); i++) {
            cout << path[i] << "->";
        }
        cout << endl; */
        unordered_map<int, vector<int>> cur_node_adj = adjNodes[current_node];

        // explore the candidates to expand from the current node
        for (auto it = cur_node_adj.begin(); it != cur_node_adj.end(); ++it) {
            int expanded = it->first;
//             cout<< "current node: "<< current_node <<"; expand node: " << expanded << endl;
//             cout << "current partition: "<< nodePartitionMap[current_node] << "; next partition: "<< nodePartitionMap[expanded] << endl; 
            
            // if current node has not been visited
            if (find(path.begin(), path.end(), expanded) == path.end()){
                
                
                bool currenInFragment;
                if (nodePartitionMap[current_node]==nodePartitionMap[expanded]){
                    if (withInFragmentFlag){
//                         cout<< "Short cut: current node: "<< current_node <<"; expand node: " << expanded << endl;
//                         cout << "current partition: "<< nodePartitionMap[current_node] << "; next partition: "<< nodePartitionMap[expanded] << endl;
//                         cout << "path->";
//                         for (int i=0; i<path.size(); i++) {
//                                 cout << path[i] << "->";
//                             }
//                         cout << endl;
                        continue;
                    }
                    else {
                        currenInFragment = true;
                    }
                }
                else {
                    currenInFragment = false;
                }
                
                
                
                // update cost
                vector <int> expanded_total_cost = {total_cost_record[total_cost_record.size()-1][0]+cur_node_adj.at(expanded)[0],
                                                    total_cost_record[total_cost_record.size()-1][1]+cur_node_adj.at(expanded)[1],
                                                    total_cost_record[total_cost_record.size()-1][2]+cur_node_adj.at(expanded)[2],
                                                    total_cost_record[total_cost_record.size()-1][3]+cur_node_adj.at(expanded)[3]};

                if (!isDominated_for_all_nodes(expanded, total_cost_record, expanded_total_cost, path)) {

                        if (expanded > current_s_node){
                            // add to the path
                            vector<int> goal_path = path;
                            goal_path.push_back(expanded);
                            // concatanate all info
                            expanded_total_cost.push_back(goal_path[0]);
                            expanded_total_cost.push_back(expanded);
                            expanded_total_cost.insert(expanded_total_cost.end(), goal_path.begin(), goal_path.end());
                            all_paths->push_back(expanded_total_cost);
                        }

                        // add to the path
                        // continue searching
                        DFS(expanded, path, total_cost_record, expanded_total_cost, all_paths, currenInFragment);
                    
                
                }
            }

        }
    }


    /*
    Input:
    The format of the map is a 2-d vector. The first dimension is the edges of the map, and the second dimension is the s_node, d_node, cost1 to cost4.

    Output:
    The obtained paths(output, returned ) should be saved in a 2-d vector, each row is a path, and each path have this format: [cost1, cost2, cost3, cost4, s_node, d_node, s_node, ......., d_node]. (eg. path with visited node 1,2,3,4,5 and costs 30,50,70,80. It will be saved as: [30,50,70,80,1,5,1,2,3,4,5]. The length of the vector is 6 + the path length.)

    Function:
    Use DFS to traverse all possible paths among one pair of nodes(s_node, d_node) of the map. The cost the the paths is the sums of each component of the cost with the edges it visited. Each time the DFS expand, call isDominated(s_node, d_node, cost1, cost2, vector<vector<int>>existingLUBs). If returned false, continue. If returned true, backtrack from the node and expand on other node.
    */
    vector<vector<int>> traverse()
    {
        vector<int> current_path;
        vector<vector<int>> total_cost_record;
        vector<int> start_cost = {0, 0, 0, 0};
        bool withInFragmentFlag = true;

        vector<vector<int>> *all_paths = new vector<vector<int>>();
        DFS(current_s_node, current_path, total_cost_record, start_cost, all_paths,withInFragmentFlag);

        return *all_paths;
    }

    /*
    input:
    The format of the map is a 2-d vector. The first dimension is the edges of the map, and the second dimension is the s_node, d_node, cost1 to cost4. 

    output: Save all the obtained paths of all pairs of nodes.

    functions:
    Enumerate all pair of s_nodes and d_nodes, call traverse( ) and save all obtained paths.
    */
    void allPairs()
    {
        // enumerate all pairs
        // for (int i=0; i<key_nodes.size()-1; i++) {
        for (int i=0; i<key_nodes.size()-1; i++) {

            // set s_node, d_node, and respective cost
            current_s_node = key_nodes[i];
            vector<vector<int>> this_pair_paths = traverse();
            if (i%20 ==0){
                cout << "done " << i << endl;
            }
            
                
            allpath.insert(allpath.end(), this_pair_paths.begin(), this_pair_paths.end());
        }
        /*
        for (int i=0; i<key_nodes.size()-1; i++) {
            for (int j=i+1; j<key_nodes.size(); j++) {

                // set s_node, d_node, and respective cost
                current_s_node = key_nodes[i];
                //current_d_node = key_nodes[j];
                s2d_existing_cost3 = existingLUBs.at(current_s_node).at(current_d_node)[2];
                s2d_existing_cost4 = existingLUBs.at(current_s_node).at(current_d_node)[3];

                vector<vector<int>> this_pair_paths = traverse();
                
                allpath.insert(allpath.end(), this_pair_paths.begin(), this_pair_paths.end());
            }
        }
        */
    }


    /* 
    input: filetype "txt" or "json"
    output: None
    function: save the result of DFS to the file with specified filetype 
    */
    void save_all_path(string filetype) {
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

//             json s_node_key;
//             for (int i=0; i<allpath.size(); i++){
//                 json d_node_key;
//                 string s_node = to_string(allpath[i][4]);
//                 string d_node = to_string(allpath[i][5]);
//                 int next_node = allpath[i][7];
//                 allpath[i].erase(allpath[i].begin()+4, allpath[i].end());
//                 allpath[i].push_back(next_node);
//                 vector<vector<int>> temp;
//                 temp.push_back(allpath[i]);

//                 // if there is no s_node as a key in json 
//                 if (!s_node_key.contains(s_node)){
//                     d_node_key[d_node] = temp;
//                     s_node_key[s_node] = d_node_key;
//                 } else {
//                     // if there is a s_node as a key but no d_node as a key
//                     if (!s_node_key[s_node].contains(d_node)){
//                         s_node_key[s_node][d_node] = temp;
//                     // if both s_node and d_noe exists as a key in json
//                     } else {
//                         s_node_key[s_node][d_node].push_back(allpath[i]);
//                     }
//                 }
//             }

//             // ATTENTION: key in the json file would be all stirngs
//             ofstream jsonfile(folderpath + "/boundaryEncodedPathView.json");
//             jsonfile << s_node_key.dump();
//             jsonfile.close();
        }
    }

    void convert_map_of_map_to_json_file() {

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
};