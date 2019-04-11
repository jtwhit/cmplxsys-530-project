#pragma once

#include "simulate.hpp"
#include <string>
#include <queue>
#include <mutex>

class SweepRunner {
public:
    void queue_sweep(const std::string &name);
    void run();

private:
    void run_thread();
    std::pair<std::string, SimParams> pop_sweep();
    bool sweeps_empty() const;

    std::queue<std::pair<std::string, SimParams>> sweeps;
    mutable std::mutex queue_mutex;
};
