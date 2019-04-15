#include "distribution.hpp"
#include <random>
#include <cmath>
#include <algorithm>

using namespace std;

pair<int, set<int>> generate_info(int min, int max, int length, double std_dev) {
    random_device rd;
    default_random_engine random_gen(rd());

    uniform_int_distribution<> uniform(min, max);
    int topic = uniform(random_gen);

    double p_total = 0;
    vector<double> p_vals;
    for (int i = min; i <= max; i++) {
        double p = exp(-pow(i - topic, 2) / (2*pow(std_dev,2)));
        p_vals.push_back(p);
        p_total += p;
    }

    set<int> data;
    uniform_real_distribution<> p_dist(0, 1);
    for (int i = 0; i < length; i++) {
        double p = p_dist(random_gen) * p_total;

        double p_sum = 0;
        for (size_t j = 0; j < p_vals.size(); j++) {
            p_sum += p_vals[j];
            if (p_sum > p) {
                data.insert(j - min);
                p_total -= p_vals[j];
                p_vals[j] = 0;
                break;
            }
        }
    }

    return make_pair(topic, data);
}
