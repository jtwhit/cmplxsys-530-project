#include "simulate.hpp"
#include "SweepRunner.hpp"
#include <iostream>
#include <experimental/filesystem>

using namespace std;
using namespace std::experimental::filesystem;

void run_parameter_sweep() {
    SweepRunner sweep_runner;

    for (const path &p : directory_iterator("params"))
        sweep_runner.queue_sweep(p.filename());

    sweep_runner.run();
}

int main() {
    run_parameter_sweep();

    return 0;
}
