#include "simulate.hpp"
#include "distribution.hpp"
#include <algorithm>
#include <fstream>
#include <functional>
#include <thread>
#include <experimental/filesystem>

using namespace std;
using namespace std::experimental::filesystem;

SimParams read_params(path param_path) {
    SimParams params;

    ifstream input(param_path);
    input >> params.num_users;
    input >> params.num_pages;
    input >> params.max_info_int;
    input >> params.page_length;
    input >> params.page_std_dev;
    input >> params.weights.page_click >> params.weights.info_found >> params.weights.topic_similarity;
    input >> params.user_length;
    input >> params.user_std_dev;
    input >> params.user_sat_pct;

    return params;
}

void run_sim(path param_path, bool print = true) {
    path output_path("outputs");
    output_path /= param_path.filename();
    if (exists(output_path)) {
        return;
    }

    SimParams params = read_params(param_path);
    vector<SimResult> run_results = simulate(params, print);

    ofstream output(output_path);
    for (SimResult result : run_results) {
        output << result.list_depth << ", " << result.pages_read << "\n";
    }
}

void run_parameter_sweep() {
    create_directory("outputs");

    vector<thread> threads;
    for (path p : directory_iterator("params")) {
        threads.emplace_back(run_sim, p, false);
    }

    for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
}

int main() {
    run_parameter_sweep();

    return 0;
}
