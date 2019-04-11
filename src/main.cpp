#include "simulate.hpp"
#include "SweepRunner.hpp"
#include <iostream>

using namespace std;

int main() {
    SweepRunner sweep_runner;
    sweep_runner.queue_sweep("num_users_10");
    sweep_runner.run();

    return 0;
}
