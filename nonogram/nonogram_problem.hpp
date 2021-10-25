#ifndef ___NONOGRAM_PROBLEM_IMPLEMENTATION___
#define ___NONOGRAM_PROBLEM_IMPLEMENTATION___

#include <vector>
#include <tuple>
#include <sstream>
#include <random>



using axis_t = std::vector<std::vector<int>>;
using clue_t = std::pair<axis_t, axis_t>;
using board_t = std::vector<std::vector<bool>>;


double cost_function(clue_t target, clue_t candidate);

clue_t board_to_clueset(board_t board);

board_t next_solution_candidate(board_t solution);

board_t gen_rand_board(int size_x, int size_y);

board_t brute_force(clue_t clueset, int iterations);

board_t brute_force(clue_t clueset);



clue_t load_clueset(std::string filename);

void print_clueset(clue_t clueset);
void print_board(board_t board);

#endif