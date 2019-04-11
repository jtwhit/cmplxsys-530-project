#include "simulate.hpp"
#include "SweepRunner.hpp"
#include <iostream>

using namespace std;

void run_parameter_sweep() {
    SweepRunner sweep_runner;

    sweep_runner.queue_sweep("num_users_10");
    sweep_runner.queue_sweep("num_users_30");
    sweep_runner.queue_sweep("num_users_100");
    sweep_runner.queue_sweep("num_users_300");
    sweep_runner.queue_sweep("num_users_1000");
    sweep_runner.queue_sweep("num_users_3000");
    sweep_runner.queue_sweep("num_users_10000");

    sweep_runner.run();
}

int main() {
    run_parameter_sweep();

    return 0;
}
