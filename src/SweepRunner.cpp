#include "SweepRunner.hpp"
#include <fstream>
#include <thread>
#include <algorithm>
#include <iostream>

using namespace std;

void run_sweep(const string &name, SimParams params) {
    vector<SimResult> run_results = simulate(params);

    ofstream output("outputs/" + name + ".txt");
    for (SimResult result : run_results) {
        output << result.list_depth << ", " << result.pages_read << "\n";
    }
}

void SweepRunner::queue_sweep(const string &name) {
    SimParams params;
    ifstream input("params/" + name + ".txt");
    input >> params.num_users;
    input >> params.num_pages;
    input >> params.max_info_int;
    input >> params.page_length;
    input >> params.page_std_dev;
    input >> params.weights.page_click >> params.weights.topic_similarity >> params.weights.info_found;
    input >> params.user_length;
    input >> params.user_std_dev;
    input >> params.user_sat_pct;

    lock_guard<mutex> queue_lock(queue_mutex);
    sweeps.push({name, params});
}

void SweepRunner::run() {
    vector<thread> threads;
    for (size_t i = 0; i < thread::hardware_concurrency(); i++)
        threads.emplace_back(&SweepRunner::run_thread, this);

    for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
}

void SweepRunner::run_thread() {
    while (!sweeps_empty()) {
        pair<string, SimParams> sweep = pop_sweep();
        run_sweep(sweep.first, sweep.second);
    }
}

pair<string, SimParams> SweepRunner::pop_sweep() {
    lock_guard<mutex> queue_lock(queue_mutex);
    pair<string, SimParams> sweep = sweeps.front();
    sweeps.pop();
    return sweep;
}

bool SweepRunner::sweeps_empty() const {
    lock_guard<mutex> queue_lock(queue_mutex);
    return sweeps.empty();
}
