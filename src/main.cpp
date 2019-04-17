#include "simulate.hpp"
#include "display.hpp"
#include "distribution.hpp"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>
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

void run_sim(path param_path, SimProgress &progress) {
    SimParams params = read_params(param_path);
    vector<SimResult> run_results = simulate(params, progress);

    path output_path("outputs");
    output_path /= param_path.filename();
    ofstream output(output_path);
    for (SimResult result : run_results) {
        output << result.list_depth << ", " << result.pages_read << "\n";
    }
}

void run_sims(const vector<path> &paths) {
    create_directory("outputs");

    vector<SimProgress> progresses(paths.begin(), paths.end());
    thread display_thread(display, ref(progresses));

    vector<thread> threads;
    set<int> running_sims;
    int sim_idx = 0;
    while (sim_idx < static_cast<int>(paths.size())) {
        if (running_sims.size() < thread::hardware_concurrency()) {
            threads.emplace_back(run_sim, paths[sim_idx], ref(progresses[sim_idx]));
            running_sims.insert(sim_idx);
            sim_idx++;
        } else {
            for (int sim : running_sims) {
                if (!progresses[sim].working()) {
                    running_sims.erase(sim);
                }
            }
            this_thread::sleep_for(chrono::milliseconds(1000));
        }
    }

    for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
    display_thread.join();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Error: Must specify at least one parameter file or directory." << endl;
        cerr << "Usage: " << argv[0] << " PARAM_PATH [PARAM_PATH ...]" << endl;
        return 1;
    }

    vector<path> paths;
    for (int i = 1; i < argc; i++) {
        path new_path(argv[i]);
        if (!exists(new_path)) {
            cerr << "Error: " << new_path << " does not exist." << endl;
            return 1;
        }

        if (is_directory(new_path)) {
            for (auto sub_path : directory_iterator(new_path)) {
                if (is_regular_file(sub_path)) {
                    if (find(paths.begin(), paths.end(), sub_path) == paths.end()) {
                        paths.push_back(sub_path);
                    }
                }
            }
        } else if (is_regular_file(new_path)) {
            if (find(paths.begin(), paths.end(), new_path) == paths.end()) {
                paths.push_back(new_path);
            }
        }
    }

    sort(paths.begin(), paths.end());
    run_sims(paths);

    return 0;
}
