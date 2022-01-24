#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <functional>
#include <chrono>

std::random_device rd;
std::mt19937 gen(rd());


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

double rastrigin(std::vector<double> v) {
    auto n = (double)v.size();
    double a = 10.0;
    double result = 0.0;
    for (auto& x : v) {
        result += pow(x, 2.0) - 10.0 * cos(2.0 * M_PI * x);
    }
    return a * n + result;
}

std::vector<double> annealing(std::function<double(std::vector<double>)> f, std::function<bool(std::vector<double>)> f_domain, std::vector<double> p0, 
    int iterations, std::function<std::vector<double>(std::vector<double>)> norm, 
    std::function<double()> temp, double t_value)
{
    auto s_current = p0;
    auto s_best = p0;

    std::uniform_real_distribution<> u_k(0.0, 1.0);

    if (!f_domain(s_current)) throw std::invalid_argument("The p0 point must be in domain");

    for (int i = 0; i < iterations; i++) {
        std::vector<double> s_new;
        do {
            s_new = norm(s_current);
        } while (!f_domain(s_new));
        if (f(s_new) < f(s_current)) {
            s_current = s_new;
        }
        else {
            double u = u_k(gen);
            if (u < std::exp(-std::abs(f(s_new) - f(s_current)) / temp())) {
                s_current = s_new;
            }
        }
        if (f(s_current) < f(s_best)) {
            s_best = s_current;
        }

        std::cout << "step " << i + 1 << std::endl;
        std::cout << "x: " << s_current.at(0) << std::endl << "y: " << s_current.at(1) << std::endl << "result: " << rastrigin(s_current) << std::endl;

    }
    return s_best;
}

int main(int argc, char** argv)
{
    using std::vector;
    using namespace std::chrono;

    std::string t_funct;
    double t_value;
    int iterations = 0;


    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-o") || (arg == "--output")) {
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
        else if ((arg == "-t") || (arg == "--temperature")) {
            if (i + 1 < argc) {
                try {
                    i++;
                    if ((strcmp(argv[i], "tempdivide") == 0) ||
                        (strcmp(argv[i], "tempdividelog") == 0) ||
                        (strcmp(argv[i], "temppow") == 0)) {
                        t_funct = std::string(argv[i]);
                    }
                    else {
                        throw std::invalid_argument("provided --temperature doesn't exist");
                    }
                }
                catch (std::invalid_argument e) {
                    std::cerr << e.what() << std::endl;
                    return 2;
                }
            }
            else {
                std::cerr << "--temperature option requires one argument" << std::endl;
                return 1;
            }
        }
        else if ((arg == "-i") || (arg == "--iterations")) {
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
        else if ((arg == "-tp") || (arg == "--temperature_parameter")) {
            if (i + 1 < argc) {
                try {
                    i++;
                    char* d;
                    t_value = atof(argv[i]);
                    if (t_value == 0.0) {
                        throw std::invalid_argument("--temperature_parameter must be a positive number");
                    }
                }
                catch (std::invalid_argument e) {
                    std::cerr << e.what() << std::endl;
                    return 2;
                }
            }
            else {
                std::cerr << "--temperature_parameter option requires one argument" << std::endl;
                return 1;
            }
        }
    }

   

    auto tempdivide = [&t_value, &iterations]() {
        return t_value / iterations;
    };

    auto temppow = [&t_value, &iterations]() {
        return pow(t_value, iterations);
    };

    auto tempdividelog = [&t_value, &iterations]() {
        return t_value / log(iterations);
    };

    auto normal = [](auto p) {
        std::normal_distribution<double> n(0.0, 0.3);
        for (auto& e : p) {
            e = e + n(gen);
        }
        return p;
    };

    
    auto rastrigin_domain = [](vector<double> v) {
        return (std::abs(v[0]) <= 5.12) && (std::abs(v[1]) <= 5.12);
    };

    std::uniform_real_distribution<> rastrigin_dist(-5.12, 5.12);

    vector<double> rastrigin_p0 = {
        rastrigin_dist(gen),
        rastrigin_dist(gen)
    };

    vector<double> next_p_dist = { -0.1, 0.1 };

    vector<double> result;
    auto st_clock_before = steady_clock::now();
    
    if (t_funct == "tempdivide") {
        result = annealing(rastrigin, rastrigin_domain, rastrigin_p0, iterations, normal, tempdivide, t_value);
    }
    else if (t_funct == "tempdividelog") {
        result = annealing(rastrigin, rastrigin_domain, rastrigin_p0, iterations, normal, tempdividelog, t_value);
    }
    else if (t_funct == "temppow") {
        result = annealing(rastrigin, rastrigin_domain, rastrigin_p0, iterations, normal, temppow, t_value);
    }

    auto st_clock_after = steady_clock::now();

    

    duration<double, std::milli> time_elapsed = st_clock_after - st_clock_before;
    std::cout << "x: " << result.at(0) << std::endl << "y: " << result.at(1) << std::endl << "result: " << rastrigin(result) << std::endl;
    print_time_elapsed(time_elapsed);
    return 0;
}
