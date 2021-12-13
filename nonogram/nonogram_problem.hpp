#ifndef ___NONOGRAM_PROBLEM_IMPLEMENTATION___
#define ___NONOGRAM_PROBLEM_IMPLEMENTATION___

#include <vector>
#include <tuple>
#include <sstream>
#include <random>
#include <functional>

#include "types.hpp"


double cost_function(clue_t target, clue_t candidate);

clue_t board_to_clueset(board_t board);

board_t next_solution_candidate(board_t solution);

std::vector<board_t> neighbour_list(board_t main_board);

board_t gen_rand_board(int size_x, int size_y);

board_t brute_force(clue_t clueset, int iterations);

board_t hillclimb(clue_t clueset, int iterations);

board_t hillclimb_stch(clue_t clueset, int iterations);

board_t tabu(clue_t clueset, int iterations, int tabu_size);

clue_t load_clueset(std::string filename);

void print_clueset(clue_t clueset);
void print_board(board_t board);

#endif