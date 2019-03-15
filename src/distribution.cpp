#include "distribution.hpp"

using namespace std;

random_device rd;
default_random_engine random_gen(rd());

int uniform_int(int min, int max) {
    uniform_int_distribution<> uniform(min, max);
    return uniform(random_gen);
}

vector<int> normal_ints(int n, int mean, double standard_dev) {
    vector<int> ints;

    normal_distribution<> norm(mean, standard_dev);
    for (int i = 0; i < n; i++) {
        int new_data = static_cast<int>(round(norm(random_gen)));
        ints.push_back(new_data);
    }

    return ints;
}

