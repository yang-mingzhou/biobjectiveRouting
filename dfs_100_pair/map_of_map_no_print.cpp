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

/*
input: The name of the file to be read.
output: A vector of vectors storing the content of the .txt file.

function:
Read the map information from a .txt file,  return a vector of vectors of the content.The format is  “s_node d_node cost1 cost2 cost3 cost4” in each line of the file.
*/

vector< vector<int> > fileToVector(const string& filename) {
    ifstream fin(filename);
    // variables
    vector< vector<int> > map_info;
    int s_node, d_node, cost1, cost2, cost3, cost4;

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
    return map_info;
}



/*
input: The input is the s_node, d_node, cost1 and cost2 of the path with the expanded node, and the existing LUB graph.
output: True or False

function:
compare the paths generated from DFS with the existingLUBs. If cost1 and cost2 are both greater or one same one greater than the cost3 and cost4 with s_node and d_node pair in the existingLUBs, return true. Otherwise, return false. 
*/

bool isDominated(int s_node, int expand_node, int d_node, int cost1, int cost2, int s2d_cost3, int s2d_cost4, unordered_map<int, vector<vector<int>>> existingLUBs) {  

    //if (existingLUBs.count(expandd

    int expand_cost3, expand_cost4, dest_cost1, dest_cost2;

    for (int i=0; i < existingLUBs.at(s_node).size(); i++) {
        if (existingLUBs.at(s_node)[i][0] == expand_node) {
            expand_cost3 = existingLUBs.at(s_node)[i][3];
            expand_cost4 = existingLUBs.at(s_node)[i][4];
        }
    }

    for (int i=0; i < existingLUBs.at(expand_node).size(); i++) {
        if (existingLUBs.at(expand_node)[i][0] == d_node) {
            dest_cost1 = existingLUBs.at(expand_node)[i][1];
            dest_cost2 = existingLUBs.at(expand_node)[i][2];
        }
    }

    if (cost1 < expand_cost3 || cost2 < expand_cost4) {
        if (cost1+dest_cost1 < s2d_cost3 || cost2+dest_cost2 < s2d_cost4) {
            cout << "cost1vs.cost3: " << cost1 <<" " << expand_cost3 << " cost2 vs.cost4: " << cost2 << " " <<  expand_cost4 << endl;
            cout << "c1+dest_c1vs.s2d_c3: " << cost1+dest_cost1 <<" " << s2d_cost3 << " c2+dest_c2vs.s2d_c4: " << cost2+dest_cost2 << " " <<  s2d_cost4 << endl;

            return false;
        }
    }
    return true;
                
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


bool isDominated_for_all_nodes(int expanded_node, vector<vector<int>> cost_record, vector<int> cur_s2e_cost, vector<int> path, unordered_map<int, unordered_map<int, vector<int>>>& existingLUBs) {

    for (int i=0; i<cost_record.size(); i++) {
        int c2e_ccost1 = cur_s2e_cost[0] - cost_record[i][0];
        int c2e_ccost2 = cur_s2e_cost[1] - cost_record[i][1];
        
        int c2e_excost3 = existingLUBs.at(path[i]).at(expanded_node)[2];
        int c2e_excost4 = existingLUBs.at(path[i]).at(expanded_node)[3];

        if (c2e_ccost1 >= c2e_excost3 && c2e_ccost2 >= c2e_excost4) {
            if (!(c2e_ccost1 == c2e_excost3 && c2e_ccost2 == c2e_excost4)) {
                return true;
            }
        }

    }
    return false;
}

bool isDominated_for_a_star(int expanded_node, int dest_node, int s2d_excost3, int s2d_excost4, vector<int> s2e_curcost, unordered_map<int, unordered_map<int, vector<int>>>& existingLUBs) {
    

    int e2d_excost1 = existingLUBs.at(expanded_node).at(dest_node)[0];
    int e2d_excost2 = existingLUBs.at(expanded_node).at(dest_node)[1];
    if (s2e_curcost[0]+e2d_excost1 < s2d_excost3 || s2e_curcost[1]+e2d_excost2 < s2d_excost4) {
        return false;
    } else {
        return true;
    }
}


/* 
 * input: current start_node, dest_node, current path from s_node, current total cost, edge index, LUBs, and vector of vector storing all path
 * 
 * function: recursive function to find all paths between specified nodes through DFS.
 */

void DFS(int cur_node, int dest_node, int s2d_c3, int s2d_c4, vector<int> path, vector<vector<int>> cost, vector<int> cur_cost, unordered_map<int, unordered_map<int, vector<int>>>& adjNodes, unordered_map<int, unordered_map<int, vector<int>>>& existingLUBs, vector<vector<int>> *all_paths)
{
    path.push_back(cur_node);
    cost.push_back(cur_cost);
    
    cout << "path->";
    for (int i=0; i<path.size(); i++) {
        cout << path[i] << "->";
    }
    cout << endl;
    unordered_map<int, vector<int>> cur_node_adj = adjNodes[cur_node];

    for (auto it = cur_node_adj.begin(); it != cur_node_adj.end(); ++it) {
        int expanded = it->first;
        // if current node has not been visited
        if (find(path.begin(), path.end(), expanded) == path.end()){
            // update cost
            vector <int> this_cost = {cost[cost.size()-1][0]+cur_node_adj.at(expanded)[0],
                                      cost[cost.size()-1][1]+cur_node_adj.at(expanded)[1],
                                      cost[cost.size()-1][2]+cur_node_adj.at(expanded)[2],
                                      cost[cost.size()-1][3]+cur_node_adj.at(expanded)[3]};

            if (!isDominated_for_a_star(expanded, dest_node, s2d_c3, s2d_c4, this_cost, existingLUBs)){
                if (!isDominated_for_all_nodes(expanded, cost, this_cost, path, existingLUBs)) {
                    // if current node is dest_node
                    if (expanded == dest_node) {
                        // add to the path
                        vector<int> goal_path = path;
                        goal_path.push_back(expanded);
                        // concatanate all info
                        this_cost.push_back(goal_path[0]);
                        this_cost.push_back(expanded);
                        this_cost.insert(this_cost.end(), goal_path.begin(), goal_path.end());
                        all_paths->push_back(this_cost);
                    } else {
                        // add to the path
                        // continue searching
                        DFS(expanded, dest_node, s2d_c3, s2d_c4, path, cost, this_cost, adjNodes, existingLUBs, all_paths);
                    }
                }
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

vector<vector<int>> traverse(int s_node, int d_node, int s2d_ub1, int s2d_ub2, unordered_map<int, unordered_map<int, vector<int>>> adjNodes, unordered_map<int, unordered_map<int, vector<int>>> Existing_LUBs)
{
    vector<int> cur_path;
    vector<vector<int>> cur_cost;
    vector<int> start_cost = {0, 0, 0, 0};

    vector<vector<int>> *all_paths = new vector<vector<int>>();
    DFS(s_node, d_node, s2d_ub1, s2d_ub2, cur_path, cur_cost, start_cost, adjNodes, Existing_LUBs, all_paths);

    return *all_paths;
}


/*
input:
The format of the map is a 2-d vector. The first dimension is the edges of the map, and the second dimension is the s_node, d_node, cost1 to cost4. 

output: Save all the obtained paths of all pairs of nodes.

functions:
Enumerate all pair of s_nodes and d_nodes, call traverse( ) and save all obtained paths.
*/

vector<vector<int>> allPairs(vector<vector<int>> mapData, vector<vector<int>>Existing_LUBs)
{
    unordered_map<int, unordered_map<int, vector<int>>> adj_nodes = createAdjNodes(mapData);
    unordered_map<int, unordered_map<int, vector<int>>> existings = createAdjNodes(Existing_LUBs);
    vector<vector<int>> allpair_paths;

    vector<int> key_nodes;
    for (auto const &pair: adj_nodes) {
        key_nodes.push_back(pair.first);
    }
    sort(key_nodes.begin(), key_nodes.end());

    // enumerate all pairs
    for (int i=0; i<key_nodes.size()-1; i++) {
        for (int j=i+1; j<key_nodes.size(); j++) {

            int s2d_cost3 = existings.at(key_nodes[i]).at(key_nodes[j])[2];
            int s2d_cost4 = existings.at(key_nodes[i]).at(key_nodes[j])[3];

            vector<vector<int>> this_pair_paths = traverse(key_nodes[i], key_nodes[j], s2d_cost3, s2d_cost4, adj_nodes, existings);
            
            allpair_paths.insert(allpair_paths.end(), this_pair_paths.begin(), this_pair_paths.end());
        }
    }
    return allpair_paths;
}


void save_all_path(vector<vector<int>> allpath, string filetype) {
    if (filetype=="txt"){
        ofstream myfile;
        myfile.open("all_paths.txt");
        for (int i=0; i<allpath.size(); i++){
            for (int j=0; j<allpath[i].size(); j++) {
                myfile << (allpath)[i][j] << " ";
            }
            myfile << "\n";
        }
        myfile.close();
    } else {
        
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

        ofstream jsonfile("all_paths.json");
        jsonfile << s_node_key.dump(4);
        jsonfile.close();
        

    }
}


int main() {
    clock_t start = clock();
    vector< vector<int> > mapData = fileToVector("../b3hepv/adjacent_LUBs.txt");
    vector< vector<int> > existingLUBs = fileToVector("../b3hepv/existingLUB.txt");

    vector<vector<int>> allpath = allPairs(mapData, existingLUBs);

    /*

    //vector<vector<int>> allpath = allPairs(mapData, existingLUBs);

    //vector<vector<int>> this_pair_paths = traverse(, j, adj_nodes, Existing_LUBs);

    unordered_map<int, unordered_map<int, vector<int>>> adj_nodes = createAdjNodes(mapData);
    unordered_map<int, unordered_map<int, vector<int>>> existings = createAdjNodes(existingLUBs);
    vector<vector<int>> allpair_paths;

    vector<int> key_nodes;
    for (auto const &pair: adj_nodes) {
        key_nodes.push_back(pair.first);
    }
    sort(key_nodes.begin(), key_nodes.end());
    clock_t end = clock(); 
    std::cout << "duration = " << (double)(end - start) / CLOCKS_PER_SEC << "sec.\n";
    cout << "adjNodes size =" << key_nodes.size() << endl;
    
    vector<vector<int>> allpath;

    vector<double> time_list;
    double total = 0;

    
    for (int i=0; i<100; i++) {
        int rand_num1 = rand() % (key_nodes.size()+1);
        int rand_num2 = rand_num1 + rand() % (key_nodes.size() - rand_num1 +1);
        rand_num1 = 123;
        rand_num2 = 145;
        int s_node_id = key_nodes[rand_num1];
        int d_node_id = key_nodes[rand_num2];
        //s_node_id = 23618;
        //d_node_id = 119416;
        // s_node_id = 406788;
        // d_node_id = 413550;
        cout << endl;
        cout << "random number : " << rand_num1 << " " << rand_num2 << endl;
        cout << "node:" << s_node_id << endl;
        cout << "node:" << d_node_id << endl;

        int s2d_cost3 = existings.at(s_node_id).at(d_node_id)[2];
        int s2d_cost4 = existings.at(s_node_id).at(d_node_id)[3];

        clock_t start = clock();
        allpath = traverse(s_node_id, d_node_id, s2d_cost3, s2d_cost4, adj_nodes, existings);
        clock_t end = clock();
        std::cout << "duration = " << (double)(end - start) / CLOCKS_PER_SEC << "sec.\n";
        double time = end-start;
        total = total + time/5.0 ;
        time_list.push_back(time);
        cout << endl;
    }
    */

    save_all_path(allpath, "json");

    //cout << "average time: " << total << endl;

    for (int i=0; i<allpath.size(); i++){
        for (int j=0; j<allpath[i].size(); j++) {
            cout << (allpath)[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}
