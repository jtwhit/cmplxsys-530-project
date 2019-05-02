#include "SingleRun.hpp"
#include "simulate.hpp"
#include <fstream>

using namespace std;

SingleRun::SingleRun(const std::string &name_, const string &path_) :
    name(name_),
    path(path_) {}

void SingleRun::execute() {
    SimParams params(path);
    vector<SimResult> run_results = simulate(params);

    ofstream output("outputs/" + name + ".csv");
    for (SimResult result : run_results) {
        output << result.list_depth << ", " << result.pages_read << "\n";
    }
}
