#ifndef ___GENETIC_IMPLEMENTATION___
#define ___GENETIC_IMPLEMENTATION___

#include <vector>
#include <tuple>
#include <sstream>
#include <random>
#include <functional>

#include "types.hpp"

double max_cost_est(clue_t target);

double fitness(clue_t target, clue_t candidate);

std::vector<board_t> selection(std::vector<board_t> population);

std::pair<board_t, board_t> cross(std::pair<board_t, board_t> parents);

board_t mutate(board_t specimen);

board_t genetic(clue_t clueset,
	std::vector<board_t> initial_pop,
	std::function<double(clue_t, clue_t)> fitness,
	std::function<std::vector<board_t>(std::vector<board_t>)> selection,
	std::function<std::pair<board_t, board_t>(std::pair<board_t, board_t>)> cross,
	std::function<board_t(board_t)> mutate,
	std::function<bool(int, std::vector<board_t>)> end_condition,
	double cross_pr = 1.0,
	double mutate_pr = 1.0
);

#endif
