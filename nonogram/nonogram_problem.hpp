#ifndef ___NONOGRAM_PROBLEM_IMPLEMENTATION___
#define ___NONOGRAM_PROBLEM_IMPLEMENTATION___

#include <vector>
#include <tuple>
#include <sstream>

using axis_t = std::vector<std::vector<int>>;
using clue_t = std::pair<axis_t, axis_t>;

using solution_t = std::vector<std::vector<bool>>;

clue_t load_clueset(std::string filename);

void print_clueset(clue_t clueset);

#endif