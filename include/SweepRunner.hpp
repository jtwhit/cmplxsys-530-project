#pragma once

#include "simulate.hpp"
#include <string>
#include <vector>
#include <queue>
#include <mutex>

template <typename T>
std::vector<SimParams> generate_param_sweep(const SimParams &all_params, T& sweep_param, std::vector<T> sweep_param_values) {
    std::vector<SimParams> param_sweep;
    for (T param_value : sweep_param_values) {
        sweep_param = param_value;
        param_sweep.push_back(all_params);
    }
    return param_sweep;
}

class SweepRunner {
public:
    void queue_sweep(const std::string name, std::vector<SimParams> params);
    void run();

private:
    struct Sweep {
        std::string name;
        std::vector<SimParams> params;
    };

    void run_thread();
    Sweep pop_sweep();
    bool sweeps_empty() const;

    std::queue<Sweep> sweeps;
    mutable std::mutex queue_mutex;

    friend void run_sweep(Sweep sweep);
};
