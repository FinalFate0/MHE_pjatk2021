#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>

#include "nonogram_problem.hpp"


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