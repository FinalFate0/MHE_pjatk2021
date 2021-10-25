#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>

#include "nonogram_problem.hpp"


int main(int argc, char** argv) {
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
        if (test_clueset.first.empty() == true && test_clueset.second.empty() == true) {
            throw std::invalid_argument("Provided clueset is invalid");
        }
    }
    catch (std::invalid_argument e) {
        std::cout << e.what() << std::endl;
        return 2;
    }


/*
    board_t generated_board = gen_rand_board(5, 5);
    board_t generated_board2 = gen_rand_board(5, 5);

    clue_t generated_clueset = board_to_clueset(generated_board);
    clue_t generated_clueset2 = board_to_clueset(generated_board2);

    print_clueset(generated_clueset);
    std::cout << std::endl;
    print_board(generated_board);

    print_clueset(generated_clueset2);

    std::cout << std::endl << std::endl << "compared board:" << std::endl;
    print_board(generated_board2);
    std::cout << "cost: " << cost_function(generated_clueset, board_to_clueset(generated_board2));
*/
    print_clueset(test_clueset);
    std::cout << std::endl;

    board_t bruteforced_solution  = brute_force(test_clueset);
    std::cout << std::endl << "best solution: " << std::endl << std::endl;
    print_clueset(board_to_clueset(bruteforced_solution));
    std::cout << std::endl;
    print_board(bruteforced_solution);
    std::cout << "cost: " << cost_function(test_clueset, board_to_clueset(bruteforced_solution)) << std::endl;

    return 0;
}