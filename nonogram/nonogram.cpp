#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <stdio.h>
#include <chrono>

#include "nonogram_problem.hpp"
#include "genetic.hpp"

void print_time_elapsed(std::chrono::duration<double, std::milli> time_elapsed) {
    using namespace std::chrono;
    auto hr = duration_cast<hours>(time_elapsed);
    auto min = duration_cast<minutes>(time_elapsed - hr);
    auto sec = duration_cast<seconds>(time_elapsed - hr - min);
    auto ms = duration_cast<milliseconds>(time_elapsed - hr - min - sec);
    auto us = duration_cast<microseconds>(time_elapsed - hr - min - sec - ms);

    std::cout << "time elapsed: "
        << hr.count() << "h:"
        << min.count() << "m:"
        << sec.count() << "s:"
        << ms.count() << "ms:"
        << us.count() << "us"
        << std::endl;
}

int main(int argc, char** argv) {

    using namespace std::chrono;

    clue_t test_clueset;
    board_t solution;
    std::string method;
    long iterations = 0;
    double target_fitness = 0.0;
    long tabu_size = 0;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-i") || (arg == "--input")) {
            if (i + 1 < argc) {
                test_clueset = load_clueset(argv[++i]);
                try {
                    if (test_clueset.first.empty() == true || test_clueset.second.empty() == true) {
                        throw std::invalid_argument("provided --clueset is invalid or missing");
                    }
                }
                catch (std::invalid_argument e) {
                    std::cerr << e.what() << std::endl;
                    std::cerr << "generating random 5x5 clueset as fallback..." << std::endl;

                    board_t generated_board = gen_rand_board(5, 5);
                    test_clueset = board_to_clueset(generated_board);
                }
            }
            else { 
                std::cerr << "--input option requires one argument" << std::endl;
                return 1;
            }
        }
        else if ((arg == "-o") || (arg == "--output")) {
            if (i + 1 < argc) {
                try {
                    std::cout << "Outputting to file..." << std::endl;
                    auto success = std::freopen(argv[++i], "w", stdout);
                    if (success == NULL) {
                        throw std::runtime_error("opening --output file failed");
                    }

                }
                catch (std::runtime_error e) {
                    std::cerr << e.what() << std::endl;
                    std::cerr << "outputting to console as fallback..." << std::endl;
                }
            }
            else {
                std::cerr << "--output option requires one argument" << std::endl;
                return 1;
            }
        }
        else if ((arg == "-m") || (arg == "--method")) {
            if (i + 1 < argc) {
                try {
                    i++;
                    if ((strcmp(argv[i], "bruteforce") == 0) || 
                        (strcmp(argv[i], "hillclimb") == 0) || 
                        (strcmp(argv[i], "hillclimb_stch") == 0) ||
                        (strcmp(argv[i], "tabu") == 0) ||
                        (strcmp(argv[i], "genetic") == 0)) {
                        method = std::string(argv[i]);
                    }
                    else {
                        throw std::invalid_argument("provided --method doesn't exist");
                    }
                }
                catch (std::invalid_argument e) {
                    std::cerr << e.what() << std::endl;
                    return 2;
                }
            }
            else {
                std::cerr << "--method option requires one argument" << std::endl;
                return 1;
            }
        }
        else if ((arg == "-it") || (arg == "--iterations")) {
            if (i + 1 < argc) {
                try {
                    i++;
                    char* d;
                    iterations = std::strtol(argv[i], &d, 10);
                    if (*d || iterations < 0) {
                        throw std::invalid_argument("--iterations must be a positive number");
                    }
                }
                catch (std::invalid_argument e) {
                    std::cerr << e.what() << std::endl;
                    return 2;
                }
            }
            else {
                std::cerr << "--iterations option requires one argument" << std::endl;
                return 1;
            }
        }
        else if ((arg == "-ts") || (arg == "--tabusize")) {
            if (i + 1 < argc) {
                try {
                    i++;
                    char* d;
                    tabu_size = std::strtol(argv[i], &d, 10);
                    if (*d || tabu_size < 0) {
                        throw std::invalid_argument("--tabusize must be a positive number");
                    }
                }
                catch (std::invalid_argument e) {
                    std::cerr << e.what() << std::endl;
                    return 2;
                }
            }
            else {
                std::cerr << "--tabusize option requires one argument" << std::endl;
                return 1;
            }
        }
    }

    //TODO: check if arguments exist

    auto st_clock_before = steady_clock::now();

    if (method == "bruteforce") {
        solution = brute_force(test_clueset, iterations);
    }
    else if (method == "hillclimb") {
        solution = hillclimb(test_clueset, iterations);
    }
    else if (method == "hillclimb_stch") {
        solution = hillclimb_stch(test_clueset, iterations);
    }
    else if (method == "tabu") {
        if (tabu_size == 0) {
            std::cerr << "\"" << method << "\" method requires --tabusize argument" << std::endl;
            return 1;
        }
        solution = tabu(test_clueset, iterations, tabu_size);
    }
    else if (method == "genetic") {
        auto end_condition_iter = [&iterations](int i, std::vector<board_t>) {
            if (i >= iterations) {
                return false;
            }
            else return true;
        };

        auto end_condition_average = [&target_fitness](int i, std::vector<board_t>) {
            //TODO
        };

        std::vector<board_t> initial_population(8, gen_rand_board(test_clueset.first.size(),
            test_clueset.second.size()));

        solution = genetic(test_clueset,
            initial_population,
            fitness,
            selection,
            cross,
            mutate,
            end_condition_iter);

    } else {
        std::cerr << "--method wasn't provided";
        return 2;
    }

    auto st_clock_after = steady_clock::now();


    //print_clueset(test_clueset);
    //std::cout << std::endl << std::endl;


    std::cout << "best solution: " << std::endl << std::endl;
    print_clueset(board_to_clueset(solution));
    std::cout << std::endl;
    print_board(solution);
    std::cout << "cost: " << cost_function(test_clueset, board_to_clueset(solution)) << std::endl << std::endl;

    duration<double, std::milli> time_elapsed = st_clock_after - st_clock_before;
    print_time_elapsed(time_elapsed);

    return 0;
}