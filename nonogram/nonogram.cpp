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

    print_clueset(test_clueset);

    return 0;
}