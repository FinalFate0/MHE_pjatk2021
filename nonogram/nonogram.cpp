#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>

using std::vector;
using std::pair;

using clue_t = pair< vector<vector<int>>, vector<vector<int>> >;

clue_t load_clue(std::string filename) {
    using namespace std;

    ifstream inputfile(filename);
    clue_t clues;
    auto clues_x = clues.first;
    auto clues_y = clues.second;

    string line;
    while (getline(inputfile, line)) {
        vector<int> clue;
        istringstream iss(line);
        int value;
        while (!iss.eof()) {

            iss >> value;
            row.push_back(value);
        }
        result.push_back(row);
    }
}




int main(int argc, char** argv) {

    return 0;
}
