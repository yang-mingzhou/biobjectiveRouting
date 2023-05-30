//convert Dimac format to Kahip format

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;


int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <inputFileName> <outputFileName>" << std::endl;
        return 1;
    }

    std::string inputFileName = argv[1];
    std::string outputFileName = argv[2];


    ifstream fin(inputFileName);
    ofstream fout(outputFileName);

    int n, m;
    fin >> n >> m;
    fout << n << " " << m/2 << endl;

    vector<vector<int> > adj_list(n+1);

    for(int i=0; i<m; i++)
    {
        int u, v, w1, w2;
        fin >> u >> v >> w1 >> w2;
        adj_list[v].push_back(u);
    }

    for(int i=1; i<=n; i++)
    {    
        for(int j=0; j<adj_list[i].size(); j++)
        {
            fout << adj_list[i][j] << " ";
        }
        fout << endl;
    }
    
    fin.close();
    fout.close();

    cout<< "kahip graph generated" <<endl; 

    return 0;

}

