#include "distribution.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <random>

using namespace std;

pair<int, set<int>> generate_info(int info_min, int info_max, int length, double std_dev) {
    // Initialize random number generator with random seed.
    random_device rd;
    default_random_engine random_gen(rd());

    // Create topic on uniform distribution between min and max.
    uniform_int_distribution<> uniform(info_min, info_max);
    int topic = uniform(random_gen);

    // The maximum distance info can be from topic before its probability of being
    // generated is smaller than the smallest value that can be represented in a double.
    // At this point, probability is essentially zero.
    // Calculated from the inverse PDF of the normal distribution.
    double max_distance = sqrt(-2*pow(std_dev,2)*log(numeric_limits<double>::min()));

    // Restrict min and max to be at most max_distance away.
    int norm_min = static_cast<int>(floor(topic - max_distance));
    norm_min = max(norm_min, info_min);

    int norm_max = static_cast<int>(ceil(topic + max_distance));
    norm_max = min(norm_max, info_max);

    // Generate probabilities for all info between min and max.
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
        // Choose a p uniformly between 0 and the sum of all info probabilities.
        double p = p_dist(random_gen) * p_total;

        double p_sum = 0;
        for (size_t j = 0; j < p_vals.size(); j++) {
            p_sum += p_vals[j];

            // Find the info where the sum of probabilities of info
            // less than or equal to it is at least p.
            if (p_sum >= p) {
                data.insert(j + norm_min);

                // Set the probability of this info to 0 so it never is chosen again.
                p_total -= p_vals[j];
                p_vals[j] = 0;
                break;
            }
        }
    }

    return make_pair(topic, data);
}
