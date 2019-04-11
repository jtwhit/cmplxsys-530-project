#include "simulate.hpp"
#include "SweepRunner.hpp"
#include <iostream>

using namespace std;

int main() {
    SimParams params = DEFAULT_PARAMS;
    vector<int> num_users = {1, 3, 10, 30, 100, 300, 1000, 3000};
    vector<SimParams> full_params = generate_param_sweep(params, params.num_users, num_users);

    SweepRunner sweep_runner;
    sweep_runner.queue_sweep("num_users", full_params);
    sweep_runner.run();

    return 0;
}
