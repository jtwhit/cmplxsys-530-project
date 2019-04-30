#include "ANTs.hpp"
#include "distribution.hpp"
#include <numeric>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <experimental/filesystem>
#include <future>

using namespace std;
using namespace std::experimental::filesystem;

const double SCORED_PROPORTION = 0.25;
const double INITIAL_TEMPERATURE = 5;
const double PERTURB_AMOUNT = 0.05;
const int MAX_CONSECUTIVE_UNCHANGED = 5;

double score_weights(SimParams params, Weights weights) {
    params.weights = weights;
    vector<SimResult> results = simulate(params);

    size_t num_considered = static_cast<size_t>(results.size() * SCORED_PROPORTION);
    int sum_depth = 0, num_depth = 0;
    for (size_t i = num_considered; i < results.size(); i++) {
        int depth = results[i].list_depth;
        if (depth <= 30) {
            sum_depth += depth;
            num_depth++;
        }
    }

    return sum_depth / static_cast<double>(num_depth);
}

Weights normalize(Weights weights) {
    double min_weight = weights.page_click;
    min_weight = min(min_weight, weights.info_found);
    min_weight = min(min_weight, weights.topic_similarity);

    if (min_weight < 0) {
        weights.page_click -= min_weight;
        weights.info_found -= min_weight;
        weights.topic_similarity -= min_weight;
    }

    double weight_sum = weights.page_click + weights.info_found + weights.topic_similarity;
    weights.page_click /= weight_sum;
    weights.info_found /= weight_sum;
    weights.topic_similarity /= weight_sum;

    return weights;
}

double create_delta() {
    return uniform_real(-PERTURB_AMOUNT, PERTURB_AMOUNT);
}

Weights get_next_weights(Weights current_weights) {
    double delta_pc = create_delta();
    double delta_if = create_delta();
    double delta_ts = create_delta();

    Weights next_weights = current_weights;
    next_weights.page_click += delta_pc;
    next_weights.info_found += delta_if;
    next_weights.topic_similarity += delta_ts;
    next_weights = normalize(next_weights);

    return next_weights;
}

double move_probability(SimParams params, Weights current_weights, Weights next_weights, double temperature) {
    auto current_handle = async(launch::async, score_weights, params, current_weights);
    auto next_handle = async(launch::async, score_weights, params, next_weights);

    double current_score = current_handle.get();
    double next_score = next_handle.get();
    double ratio = current_score / next_score;

    return pow(ratio, 1.0 / temperature);
}

Weights optimize_weights(const SimParams &params) {
    Weights weights = params.weights;

    int consecutive_unchanged = 0;
    double temperature = INITIAL_TEMPERATURE;
    while (consecutive_unchanged < MAX_CONSECUTIVE_UNCHANGED) {
        cout << "Temperature: " << temperature << " Weights: (" << weights.page_click << ", " << weights.info_found << ", " << weights.topic_similarity << ")" << endl;

        Weights next_weights = get_next_weights(weights);
        double prob = move_probability(params, weights, next_weights, temperature);

        if (prob > uniform_real(0, 1)) {
            weights = next_weights;
            consecutive_unchanged = 0;
        } else {
            consecutive_unchanged++;
        }
        temperature /= 1.1;
    }

    return weights;
}

SimParams read_params(path param_path) {
    SimParams params;
    params.name = param_path.string();

    ifstream input(param_path);
    input >> params.num_users;
    input >> params.num_pages;
    input >> params.max_info_int;
    input >> params.page_length_min >> params.page_length_max;
    input >> params.page_std_dev_min >> params.page_std_dev_max;
    input >> params.weights.page_click >> params.weights.info_found >> params.weights.topic_similarity;
    input >> params.user_length_min >> params.user_length_max;
    input >> params.user_std_dev_min >> params.user_std_dev_max;
    input >> params.user_sat_pct_min >> params.user_sat_pct_max;

    return params;
}

int main() {
    SimParams params = read_params("params/ants.txt");
    optimize_weights(params);
    return 0;
}
