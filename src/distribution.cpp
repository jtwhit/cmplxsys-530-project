#include "distribution.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <random>

using namespace std;
pair<int, set<int>> generate_info(int info_min, int info_max, int length, double std_dev) {
    random_device rd;
    default_random_engine random_gen(rd());

    uniform_int_distribution<> uniform(info_min, info_max);
    int topic = uniform(random_gen);

    double max_distance = sqrt(-2*pow(std_dev,2)*log(numeric_limits<double>::min()));

int norm_min = static_cast<int>(floor(topic - max_distance));
    norm_min = max(norm_min, info_min);

    int norm_max = static_cast<int>(ceil(topic + max_distance));
    norm_max = min(norm_max, info_max);

    double p_total = 0;
    vector<double> p_vals;
    for (int i = norm_min; i <= norm_max; i++) {
        double p = exp(-pow(i-topic, 2) / (2*pow(std_dev,2)));
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
            if (p_sum >= p) {
                data.insert(j + norm_min);
                p_total -= p_vals[j];
                p_vals[j] = 0;
                break;
            }
        }
    }

    return make_pair(topic, data);
}
