#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>

using std::vector;

using clue_t = std::pair< vector<vector<int>>, vector<vector<int>> >;
using axis_t = vector<vector<int>>;
//using board_t = vector<vector<bool>>;

clue_t load_clueset(std::string filename) {
    using namespace std;

    ifstream inputfile(filename);
    string line;

    axis_t clueset_x, clueset_y;

    bool is_y_axis = false;

    while (getline(inputfile, line)) {
        if(line.find(";") != string::npos) {
            is_y_axis = true;
        } else {
            int temp;
            vector<int> clue;
            istringstream iss(line);

            while (iss >> temp) {
                clue.push_back(temp);
            }

            if(!is_y_axis)
                clueset_x.push_back(clue);
            else
                clueset_y.push_back(clue);
        }
    }

    return make_pair(clueset_x, clueset_y);
}

void print_clueset(clue_t clueset) {
    using namespace std;
    axis_t clueset_x = clueset.first, clueset_y = clueset.second;

    cout << "--start of clueset--" << endl;
    for(auto line : clueset_x) {
        for(auto value : line) {
            cout << value << " ";
        }
        cout << endl;
    }
    cout << ";" << endl;
    for(auto line : clueset_y) {
        for(auto value : line) {
            cout << value << " ";
        }
        cout << endl;
    }

    cout << "--end of clueset--" << endl << endl;
    cout << "grid size: " << clueset_x.size() << "x" << clueset_y.size() << endl;
}

int main(int argc, char** argv) { 
    clue_t test_clueset;
    
    try {
        test_clueset = load_clueset(argv[1]);
    } catch(std::logic_error) {
        throw std::invalid_argument("No filename given");
    }

    if(test_clueset.first.empty() == true && test_clueset.second.empty() == true)
        throw std::invalid_argument("Provided clueset is invalid");

    print_clueset(test_clueset);

    return 0;
}