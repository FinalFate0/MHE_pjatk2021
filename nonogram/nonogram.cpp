#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <stdio.h>
#include <chrono>

#include "nonogram_problem.hpp"


int main(int argc, char** argv) {

    using namespace std::chrono;

    std::cout << "Current output: console" <<std::endl << "Output to file specified in argument instead? [y/n] ";
    std::string user_input;
    std::cin >> user_input;

    if (user_input == "y") {
        std::freopen(argv[2], "w", stdout);
    }

/*
    // generate random clueset

    board_t generated_board = gen_rand_board(10, 10);
    clue_t test_clueset = board_to_clueset(generated_board);
*/


    //load clueset from file
    clue_t test_clueset;
    try {
        if (argv[1] != NULL) {
            test_clueset = load_clueset(argv[1]);
        }
        else {
            throw std::invalid_argument("No filename given");
        }
    }
    catch (std::invalid_argument e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    try {
        if (test_clueset.first.empty() == true || test_clueset.second.empty() == true) {
            throw std::invalid_argument("Provided clueset is invalid");
        }
    }
    catch (std::invalid_argument e) {
        std::cout << e.what() << std::endl;
        return 2;
    }

    //print_clueset(test_clueset);
    //std::cout << std::endl << std::endl;

    //brute force loaded clueset
    auto st_clock_before = steady_clock::now();

    board_t bruteforced_solution  = brute_force(test_clueset);

    auto st_clock_after = steady_clock::now();

    std::cout << "best solution: " << std::endl << std::endl;
    print_clueset(board_to_clueset(bruteforced_solution));
    std::cout << std::endl;
    print_board(bruteforced_solution);
    std::cout << "cost: " << cost_function(test_clueset, board_to_clueset(bruteforced_solution)) << std::endl << std::endl;

    duration<double, std::milli> time_elapsed = st_clock_after - st_clock_before;
    auto hr = duration_cast<hours>(time_elapsed);
    auto min = duration_cast<minutes>(time_elapsed-hr);
    auto sec = duration_cast<seconds>(time_elapsed-hr-min);
    auto ms = duration_cast<milliseconds>(time_elapsed-hr-min-sec);
    auto us = duration_cast<microseconds>(time_elapsed-hr-min-sec-ms);

    std::cout << "time elapsed: "
        << hr.count() << "h:"
        << min.count() << "m:"
        << sec.count() << "s:"
        << ms.count() << "ms:"
        << us.count() << "us"
        << std::endl;

    return 0;
}