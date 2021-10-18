#include "nonogram_problem.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

clue_t load_clueset(std::string filename) {
    using namespace std;

    ifstream inputfile(filename);
    string line;

    axis_t clueset_x, clueset_y;

    bool is_y_axis = false;

    while (getline(inputfile, line)) {
        if (line.find(";") != string::npos) {
            is_y_axis = true;
        }
        else {
            int temp;
            vector<int> clue;
            istringstream iss(line);

            while (iss >> temp) {
                clue.push_back(temp);
            }

            if (!is_y_axis)
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
    for (auto line : clueset_x) {
        for (auto value : line) {
            cout << value << " ";
        }
        cout << endl;
    }
    cout << ";" << endl;
    for (auto line : clueset_y) {
        for (auto value : line) {
            cout << value << " ";
        }
        cout << endl;
    }

    cout << "--end of clueset--" << endl << endl;
    cout << "grid size: " << clueset_x.size() << "x" << clueset_y.size() << endl;
}