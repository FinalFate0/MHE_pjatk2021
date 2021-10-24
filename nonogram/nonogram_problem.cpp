#include "nonogram_problem.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

double cost_function(clue_t target, clue_t candidate) {
    double cost = 0.0;

    axis_t target_x = target.first, target_y = target.second;
    axis_t candidate_x = candidate.first, candidate_y = candidate.second;
    int size_x = target_x.size(), size_y = target_y.size();

    for (int i = 0; i < size_x; i++) {
        int sum_target = 0, sum_candidate = 0;
        int count_target = 0, count_candidate = 0;
        if (candidate_x.at(i) == target_x.at(i)) {
            cost += 0.0;
        }
        else {
            for (auto& x : target_x.at(i)) {
                sum_target += x;
                count_target++;
            }
            for (auto& x : candidate_x.at(i)) {
                sum_candidate += x;
                count_candidate++;
            }

            double sum_difference = std::abs(sum_candidate - sum_target);
            double sum_evaluation = sum_difference / size_x;

            double count_difference = std::abs(count_candidate - count_target);
            double count_evaluation = count_difference / size_x;

            cost += sum_evaluation + count_evaluation;
        }
    }

    for (int i = 0; i < size_y; i++) {
        int sum_target = 0, sum_candidate = 0;
        int count_target = 0, count_candidate = 0;
        if (candidate_y.at(i) == target_y.at(i)) {
            cost += 0.0;
        }
        else {
            for (auto& y : target_y.at(i)) {
                sum_target += y;
                count_target++;
            }
            for (auto& y : candidate_y.at(i)) {
                sum_candidate += y;
                count_candidate++;
            }

            double sum_difference = std::abs(sum_candidate - sum_target);
            double sum_evaluation = sum_difference / size_y;

            double count_difference = std::abs(count_candidate - count_target);
            double count_evaluation = count_difference / size_y;

            cost += sum_evaluation + count_evaluation;
        }
    }
    return cost;
}


clue_t board_to_clueset(board_t board) {
    int size_x = board.at(0).size();
    int size_y = board.size();

    axis_t result_x, result_y;

    for (int i = 0; i < size_x; i++) {
        std::vector<int> clue = { 0 };
        bool line = false;

        for (int j = 0; j < size_y; j++) {
            if (board.at(i).at(j) == true && line == false) {
                line = true;
                if (clue.at(i) != 0) {
                    clue.push_back(0);
                }
                clue.at(i)++;
            }
            else if (board.at(i).at(j) == true && line == true) {
                clue.at(i)++;
            }
            else if (board.at(i).at(j) == false && line == true) {
                line = false;
            }
        }
        result_x.push_back(clue);
    }

    for (int i = 0; i < size_y; i++) {
        std::vector<int> clue = { 0 };
        bool line = false;

        for (int j = 0; j < size_x; j++) {
            if (board.at(j).at(i) == true && line == false) {
                line = true;
                if (clue.at(i) != 0) {
                    clue.push_back(0);
                }
                clue.at(i)++;
            }
            else if (board.at(j).at(i) == true && line == true) {
                clue.at(i)++;
            }
            else if (board.at(j).at(i) == false && line == true) {
                line = false;
            }
        }
        result_y.push_back(clue);
    }

    return std::make_pair(result_x, result_y);
}

board_t next_solution_candidate(board_t solution) {

    //std::next_permutation(solution.begin(), solution.end());

}

board_t brute_force(clue_t clueset) {
    int size_x = clueset.first.size(), size_y = clueset.second.size();
    board_t solution(size_y, std::vector<bool>(size_x, false));

}

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
    cout << "board size: " << clueset_x.size() << "x" << clueset_y.size() << endl;
}