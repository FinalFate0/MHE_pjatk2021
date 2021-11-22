#include "nonogram_problem.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <deque>

std::random_device rd;
std::mt19937 gen(rd());

double cost_function(clue_t target, clue_t candidate) {
    double cost = 0.0;

    axis_t target_x = target.first, target_y = target.second;
    axis_t candidate_x = candidate.first, candidate_y = candidate.second;
    int size_x = target_x.size(), size_y = target_y.size();

    //compare every position, clue count and sum in every row and column

    for (int i = 0; i < size_x; i++) {
        int sum_target = 0, sum_candidate = 0;
        int count_target = 0, count_candidate = 0;
        if (candidate_x.at(i) == target_x.at(i)) {
            cost += 0.0;
        }
        else {
            int bigger_size;
            if (candidate_x.at(i).size() > target_x.at(i).size()) {
                bigger_size = candidate_x.at(i).size();
            }
            else {
                bigger_size = target_x.at(i).size();;
            }
            for (int j = 0; j < bigger_size; j++) {
                if (j >= candidate_x.at(i).size()) {
                    cost += (target_x.at(i).at(j));
                }
                else if (j >= target_x.at(i).size()) {
                    cost += (candidate_x.at(i).at(j));
                }
                else {
                    cost += (std::abs(target_x.at(i).at(j) - candidate_x.at(i).at(j)));
                }
            }

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
            int bigger_size;
            if (candidate_y.at(i).size() > target_y.at(i).size()) {
                bigger_size = candidate_y.at(i).size();
            }
            else {
                bigger_size = target_y.at(i).size();;
            }

            for (int j = 0; j < bigger_size; j++) {
                if (j >= candidate_y.at(i).size()) {
                    cost += (target_y.at(i).at(j));
                }
                else if (j >= target_y.at(i).size()) {
                    cost += (candidate_y.at(i).at(j));
                }
                else {
                    cost += (std::abs(target_y.at(i).at(j) - candidate_y.at(i).at(j)));
                }
            }

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
        int index = 0;
        for (int j = 0; j < size_y; j++) {
            if (board.at(j).at(i) == true && line == false) {
                line = true;
                if (index >= clue.size() || clue.at(index) != 0) {
                    clue.push_back(0);
                }

                clue.at(index)++;
            }
            else if (board.at(j).at(i) == true && line == true) {
                clue.at(index)++;
            }
            else if (board.at(j).at(i) == false && line == true) {
                line = false;
                index++;
            }
        }
        result_x.push_back(clue);
    }

    for (int i = 0; i < size_y; i++) {
        std::vector<int> clue = { 0 };
        bool line = false;
        int index = 0;
        for (int j = 0; j < size_x; j++) {
            if (board.at(i).at(j) == true && line == false) {
                line = true;
                if (index >= clue.size() || clue.at(index) != 0) {
                    clue.push_back(0);
                }
                clue.at(index)++;
            }
            else if (board.at(i).at(j) == true && line == true) {
                clue.at(index)++;
            }
            else if (board.at(i).at(j) == false && line == true) {
                line = false;
                index++;
            }
        }
        result_y.push_back(clue);
    }

    return std::make_pair(result_x, result_y);
}



board_t next_solution_candidate(board_t working_board) {
    board_t next_candidate = working_board;
    int size_x = next_candidate.at(0).size();
    int size_y = next_candidate.size();

    // idx =  y*szerokosc + x

    //x, y <- idx
    // x = idx % szerokosc
    // y = idx / szerokosc // bez u³amków

    for (int i = 0; i < size_x*size_y; i++) {
        int idx = i % size_x;
        int idy = i / size_x;
        if (next_candidate.at(idy).at(idx) == false) {
            next_candidate.at(idy).at(idx) = true;
            break;
        }
        else {
            next_candidate.at(idy).at(idx) = false;
        }
    }
    return next_candidate;
}

std::vector<board_t> neighbour_list(board_t main_board) {
    //list of mirrors of current board with one bit switched (25 if 5x5)

    int size_x = main_board.at(0).size(), size_y = main_board.size();
    std::vector<board_t> neighbours;
    for (int i = 0; i < size_x * size_y; i++) {
        int idx = i % size_x;
        int idy = i / size_x;
        board_t temp_board = main_board;
        temp_board.at(idy).at(idx) = !temp_board.at(idy).at(idx);
        neighbours.push_back(temp_board);
    }
    return neighbours;
}

board_t brute_force(clue_t clueset, int iterations) {
    using namespace std;

    int size_x = clueset.first.size(), size_y = clueset.second.size();
    board_t candidate(size_y, std::vector<bool>(size_x, false));

    double best_cost = 100000;
    board_t best_solution;

    //print_board(candidate);
    //cout << "cost: " << cost_function(clueset, board_to_clueset(candidate)) << endl;

    //cout << endl;
    if (iterations == 0) {
        for (int i = 0; i < pow(2, size_x * size_y); i++) {
            candidate = next_solution_candidate(candidate);
            //print_board(candidate);
            double candidate_cost = cost_function(clueset, board_to_clueset(candidate));
            //cout << "cost: " << candidate_cost << endl;
            if (candidate_cost < best_cost) {
                best_solution = candidate;
                best_cost = candidate_cost;
                if (best_cost == 0) {
                    break;
                }
            }
        }
    }
    else {
        for (int i = 0; i < iterations; i++) {
            candidate = next_solution_candidate(candidate);
            //print_board(candidate);
            double candidate_cost = cost_function(clueset, board_to_clueset(candidate));
            //cout << "cost: " << candidate_cost << endl;
            if (candidate_cost < best_cost) {
                best_solution = candidate;
                best_cost = candidate_cost;
                if (best_cost == 0) {
                    break;
                }
            }
        }
    }

    return best_solution;
}

board_t hillclimb(clue_t clueset, int iterations) {
    using namespace std;

    int size_x = clueset.first.size(), size_y = clueset.second.size();

    board_t current_board = gen_rand_board(size_x, size_y);
    double best_cost = cost_function(clueset, board_to_clueset(current_board));

    bool found_better = true;
    int i = 0;

 
    while (found_better && i < iterations) {
        vector<board_t> neighbors = neighbour_list(current_board);

        //print_board(current_board);
        //cout << "cost: " << best_cost << endl << endl;

        found_better = false;
        for (int j = 0; j < neighbors.size(); j++) {
            double current_cost = cost_function(clueset, board_to_clueset(neighbors.at(j)));

            if (current_cost < best_cost) {
                best_cost = current_cost;
                current_board = neighbors.at(j);
                found_better = true;
                break;
            }
        }
        i++;
    }
    return current_board;
}

board_t hillclimb_stch(clue_t clueset, int iterations) {
    using namespace std;

    int size_x = clueset.first.size(), size_y = clueset.second.size();
    uniform_int_distribution<> distrib(0, (size_x*size_y)-1);

    board_t current_board = gen_rand_board(size_x, size_y);
    double best_cost = cost_function(clueset, board_to_clueset(current_board));

    bool found_better = true;
    while (found_better) {
        vector<board_t> neighbors = neighbour_list(current_board);

        //print_board(current_board);
        //cout << "cost: " << best_cost << endl << endl;

        found_better = false;
        for (int i = 0; i < iterations; i++) {
            auto generated_neighbour = neighbors.at(distrib(gen));
            double current_cost = cost_function(clueset, board_to_clueset(generated_neighbour));

            if (current_cost < best_cost) {
                best_cost = current_cost;
                current_board = generated_neighbour;
                found_better = true;
                break;
            }
        }
    }
    return current_board;
}

board_t tabu(clue_t clueset, int iterations, int tabu_size) {
    using namespace std;

    int size_x = clueset.first.size(), size_y = clueset.second.size();

    board_t current_board = gen_rand_board(size_x, size_y);
    double current_cost = cost_function(clueset, board_to_clueset(current_board));
    board_t best_board = current_board;
    double best_cost = current_cost;

    deque<board_t> tabu;

    int i = 0;

    while (i < iterations && best_cost > 0) {
        vector<board_t> neighbors = neighbour_list(current_board);

        auto iterator = neighbors.begin();
        while (iterator != neighbors.end()) {
            if (find(tabu.begin(), tabu.end(), *iterator) != tabu.end()) {
                //cout << "board already in tabu" << endl << endl;
                iterator = neighbors.erase(iterator);
            }
            else {
                ++iterator;
            }
        }
        if (neighbors.size() == 0) {
            cout << "all candidates already in tabu; ending..." << endl << endl;
            break;
        }

        //print_board(current_board);
        //cout << "cost: " << current_cost << endl << endl;

        tabu.push_back(current_board);
        //cout << "adding board to tabu..." << endl << endl;

        board_t best_candidate = neighbors.at(0);
        double best_candidate_cost = cost_function(clueset, board_to_clueset(best_candidate));

        int hit_count = 0;

        for (int j = 0; j < neighbors.size(); j++) {
            board_t current_candidate = neighbors.at(j);
            double current_candidate_cost = cost_function(clueset, board_to_clueset(neighbors.at(j)));

            if (current_candidate_cost < best_candidate_cost) {
                best_candidate_cost = current_candidate_cost;
                best_candidate = current_candidate;
            }
        }

        current_board = best_candidate;
        current_cost = cost_function(clueset, board_to_clueset(current_board));

        if (tabu.size() > tabu_size) {
            tabu.pop_front();
            //cout << "tabu max reached; erasing oldest entry" << endl << endl;
        }
        if (best_candidate_cost < best_cost) {
            best_board = best_candidate;
            best_cost = best_candidate_cost;
        }

        i++;
        if (i % 1000 == 0) {
            cout << "iteration " << i << endl;
        }
        if (i == iterations) {
            cout << "max iteration reached; ending..." << endl << endl;
        }
        if (best_cost == 0) {
            cout << "solution found; ending..." << endl << endl;
        }
    }
    return best_board;
}

board_t gen_rand_board(int size_x, int size_y) {
    std::uniform_int_distribution<> distrib(0, 1);

    board_t board(size_y, std::vector<bool>(size_x, false));
    for (int i = 0; i < size_y; i++) {
        for (int j = 0; j < size_x; j++) {
            if (distrib(gen) == 1) {
                board.at(i).at(j) = true;
            }
        }
    }

    return board;
}

clue_t load_clueset(std::string filename) {
    using namespace std;

    ifstream inputfile(filename);
    string line;

    axis_t clueset_x, clueset_y;

    bool is_x_axis = false;

    while (getline(inputfile, line)) {
        if (line.find(";") != string::npos) {
            is_x_axis = true;
        }
        else {
            int temp;
            vector<int> clue;
            istringstream iss(line);

            while (iss >> temp) {
                clue.push_back(temp);
            }

            if (!is_x_axis)
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

void print_board(board_t board) {
    using namespace std;

    int size_x = board.at(0).size();
    int size_y = board.size();

    //extended ascii
    //char filled = char(219);
    //char empty = char(176);

    const char filled_left = '[';
    const char filled_right = ']';
    const char empty_left = '>';
    const char empty_right = '<';


    for (int i = 0; i < size_y; i++) {

        for (int j = 0; j < size_x; j++) {
            if (board.at(i).at(j) == true) {
                cout << filled_left << filled_right;
            }
            else {
                cout << empty_left << empty_right;
            }
        }
        cout << endl;
    }
}