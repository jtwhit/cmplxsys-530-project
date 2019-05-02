#include "SweepRun.hpp"
#include "simulate.hpp"
#include <fstream>
#include <cmath>

using namespace std;

const size_t LAST_N_USERS = 500;

template <typename T>
void sweep_param(ifstream &input, Param sweep_param, const string &name) {
    vector<T> values;
    T value;
    while (input >> value) {
        values.push_back(value);
    }

    SimParams params;
    vector<pair<double, double>> means;
    for (T value : values) {
        params.set(sweep_param, value);
        vector<SimResult> run_results = simulate(params);

        int last_n = min(LAST_N_USERS, run_results.size());
        pair<int, int> sums;
        for (size_t i = run_results.size() - last_n; i < run_results.size(); i++) {
            SimResult result = run_results[i];
            sums.first += result.list_depth;
            sums.second += result.pages_read;
        }

        pair<double, double> new_means;
        new_means.first = static_cast<double>(sums.first) / last_n;
        new_means.second = static_cast<double>(sums.second) / last_n;
        means.push_back(new_means);
    }

    ofstream output("outputs/" + name + ".csv");
    for (size_t i = 0; i < values.size(); i++) {
        output << values[i] << "," << means[i].first << "," << means[i].second << "\n";
    }
}

SweepRun::SweepRun(const std::string &name_, const string &path_) :
    name(name_),
    path(path_) {}

void SweepRun::execute() {
    ifstream input(path);
    string param_key;
    input >> param_key;
    Param param = get_param(param_key);

    if (is_integer_param(param)) {
        sweep_param<int>(input, param, name);
    } else {
        sweep_param<double>(input, param, name);
    }
}
