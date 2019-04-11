#include "SweepRunner.hpp"
#include <fstream>
#include <thread>
#include <algorithm>
#include <iostream>

using namespace std;

string weight_print_string(Weights weights) {
    return to_string(weights.page_click) + "," + to_string(weights.topic_similarity) + "," + to_string(weights.info_found);
}

string param_print_string(SimParams params) {
    return "[" + to_string(params.num_users) + " " +
                 to_string(params.num_pages) + " " +
                 to_string(params.max_info_int) + " " +
                 to_string(params.page_length) + " " +
                 to_string(params.page_std_dev) + " " +
                 weight_print_string(params.weights) + " " +
                 to_string(params.user_length) + " " +
                 to_string(params.user_std_dev) + " " +
                 to_string(params.user_sat_pct) + "]";
}

void run_sweep(SweepRunner::Sweep sweep) {
    for (SimParams params : sweep.params) {
        vector<SimResult> run_results = simulate(params, true);

        ofstream output(sweep.name + "_" + param_print_string(params) + ".txt");
        for (SimResult result : run_results) {
            output << result.list_depth << ", " << result.pages_read << "\n";
        }
    }
}

void SweepRunner::queue_sweep(const std::string name, std::vector<SimParams> params) {
    lock_guard<mutex> queue_lock(queue_mutex);
    sweeps.push({name, params});
}

void SweepRunner::run() {
    vector<thread> threads;
    for (size_t i = 0; i < thread::hardware_concurrency(); i++) {}
        threads.emplace_back(&SweepRunner::run_thread, this);

    for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
}

void SweepRunner::run_thread() {
    while (!sweeps_empty()) {
        Sweep sweep = pop_sweep();
        run_sweep(sweep);
    }
}

SweepRunner::Sweep SweepRunner::pop_sweep() {
    lock_guard<mutex> queue_lock(queue_mutex);
    Sweep sweep = sweeps.front();
    sweeps.pop();
    return sweep;
}

bool SweepRunner::sweeps_empty() const {
    lock_guard<mutex> queue_lock(queue_mutex);
    return sweeps.empty();
}
