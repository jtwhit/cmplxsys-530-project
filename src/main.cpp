#include "simulate.hpp"
#include "distribution.hpp"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <set>
#include <thread>
#include <experimental/filesystem>

using namespace std;
using namespace std::experimental::filesystem;

const int PRINT_DELAY_MS = 1000;

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

void run_sims(const set<path> &paths) {
    create_directory("outputs");

    vector<SimProgress> progresses(paths.begin(), paths.end());
    vector<thread> threads;
    int progress_idx = 0;
    for (path p : paths) {
        threads.emplace_back(run_sim, p, ref(progresses[progress_idx]));
        progress_idx++;
    }


    while (any_of(progresses.begin(), progresses.end(), mem_fn(&SimProgress::working))) {
        for (SimProgress &progress : progresses) {
            cout << progress << endl;
        }
        cout << "\033[" << progresses.size() << "F";

        this_thread::sleep_for(chrono::milliseconds(PRINT_DELAY_MS));
    }
    cout << endl;

    for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Error: Must specify at least one parameter file or directory." << endl;
        cerr << "Usage: " << argv[0] << " PARAM_PATH [PARAM_PATH ...]" << endl;
        return 1;
    }

    set<path> paths;
    for (int i = 1; i < argc; i++) {
        path new_path(argv[i]);
        if (!exists(new_path)) {
            cerr << "Error: " << new_path << " does not exist." << endl;
            return 1;
        }

        if (is_directory(new_path)) {
            for (auto sub_path : directory_iterator(new_path)) {
                if (is_regular_file(sub_path)) {
                    paths.insert(sub_path);
                }
            }
        } else if (is_regular_file(new_path)) {
            paths.insert(new_path);
        }
    }

    run_sims(paths);

    return 0;
}
