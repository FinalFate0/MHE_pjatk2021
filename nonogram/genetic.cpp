#include <vector>
#include <tuple>
#include <sstream>
#include <random>
#include <functional>

#include "genetic.hpp"
#include "nonogram_problem.hpp"

double max_cost_est(clue_t target) {
    double cost = 0.0;

    axis_t target_x = target.first, target_y = target.second;
    int size_x = target_x.size(), size_y = target_y.size();


    for (int i = 0; i < size_x; i++) {
        int sum_target = 0, sum_candidate = 0;
        int count_target = 0, count_candidate = 0;

        for (int j = 0; j < size_y; j++) {
            if (j >= target_x.at(i).size()) {
                cost += 9.0;
            }
            else {
                cost += (std::abs(target_x.at(i).at(j) - 9.0));
            }
            sum_candidate += 9.0;
            count_candidate++;
        }
        for (auto& x : target_x.at(i)) {
            sum_target += x;
            count_target++;
        }

        double sum_difference = std::abs(sum_candidate - sum_target);
        double sum_evaluation = sum_difference / size_x;

        double count_difference = std::abs(count_candidate - count_target);
        double count_evaluation = count_difference / size_x;

        cost += sum_evaluation + count_evaluation;
    }


    for (int i = 0; i < size_y; i++) {
        int sum_target = 0, sum_candidate = 0;
        int count_target = 0, count_candidate = 0;
        for (int j = 0; j < size_x; j++) {
            if (j >= target_y.at(i).size()) {
                cost += 9.0;
            }
            else {
                cost += (std::abs(target_y.at(i).at(j) - 9.0));
            }
            sum_candidate += 9.0;
            count_candidate++;
        }
        for (auto& y : target_y.at(i)) {
            sum_target += y;
            count_target++;
        }

        double sum_difference = std::abs(sum_candidate - sum_target);
        double sum_evaluation = sum_difference / size_y;

        double count_difference = std::abs(count_candidate - count_target);
        double count_evaluation = count_difference / size_y;

        cost += sum_evaluation + count_evaluation;

    }
    return cost;
}

double fitness(clue_t target, clue_t candidate) {
	//fitness() = CONST-cost_function(x)
	//max_cost_est(problem) = const
	// 9 9 9 9 9 = [ . . . . . ]

	double max_cost = max_cost_est(target);

	//double result = 1/(cost_function(target, candidate)+0.001));
	double result = max_cost - cost_function(target, candidate);

	return result;
}

std::vector<board_t> selection(std::vector<board_t> population) {
	//TODO
	return population;
}

std::pair<board_t, board_t> cross(std::pair<board_t, board_t> parents) {
	//TODO
	return parents;
}

board_t mutate(board_t specimen) {
	//TODO
	return specimen;
}

board_t genetic(clue_t clueset,
    std::vector<board_t> initial_pop,
    std::function<double(clue_t, clue_t)> fitness,
    std::function<std::vector<board_t>(std::vector<board_t>)> selection,
    std::function<std::pair<board_t, board_t>(std::pair<board_t, board_t>)> cross,
    std::function<board_t(board_t)> mutate,
    std::function<bool(int, std::vector<board_t>)> end_condition,
    double cross_pr,
    double mutate_pr) {

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<> distrib_real(0.0, 1.0);
    std::uniform_int_distribution<> distrib_pop(0, initial_pop.size()-1);

    std::vector<board_t> current_pop = initial_pop;
    int i = 0;
    while (end_condition(i, current_pop)) {
        std::vector<board_t> new_pop = selection(current_pop);
        
        if (distrib_real(gen) <= cross_pr) {
            int index1 = distrib_pop(gen);
            board_t candidate1 = new_pop.at(index1);

            int index2;
            board_t candidate2;
            do {
                index2 = distrib_pop(gen);
            } while (index1 != index2);
            candidate2 = new_pop.at(index2);
            auto crossed = cross(std::make_pair(candidate1, candidate2));
            new_pop.at(index1) = crossed.first;
            new_pop.at(index2) = crossed.second;
        }
        if (distrib_real(gen) <= mutate_pr) {
            mutate(new_pop.at(distrib_pop(gen)));
        }


        current_pop = new_pop;
        i++;
    }

    board_t best_candidate;
    double best_candidate_fitness = 0;

    for (int i = 0; i < current_pop.size(); i++) {
        if (fitness(clueset, board_to_clueset(current_pop.at(i))) > best_candidate_fitness) {
            best_candidate = current_pop.at(i);
            best_candidate_fitness = fitness(clueset, board_to_clueset(current_pop.at(i)));
        }
    }
    return best_candidate;
}
