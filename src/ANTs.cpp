#include "ANTs.hpp"
#include "simulate.hpp"
#include "distribution.hpp"
#include <iostream>
#include <cmath>
#include <future>

using namespace std;

const double INITIAL_TEMPERATURE = 1;
const double TEMP_DECREASE_RATE = 1.01;
const double PERTURB_AMOUNT = 0.05;
const int NUM_RUNS = 100;

double score_weights(SimParams params, Weights weights) {
    params.weights = weights;

    int sum_read = 0, num_read = 0;
    for (int i = 0; i < NUM_RUNS; i++) {
        vector<SimResult> results = simulate(params);
        for (size_t i = 0; i < results.size(); i++) {
            int read = results[i].pages_read;
            sum_read += read;
            num_read++;
        }
    }

    return sum_read / static_cast<double>(num_read);
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
    cout << current_score << " " << next_score << endl;
    double ratio = current_score / next_score;

    return pow(ratio, 1.0 / temperature);
}

Weights optimize_weights(const SimParams &params) {
    Weights weights = params.weights;

    double temperature = INITIAL_TEMPERATURE;
    while (true) {
        cout << "Temperature: " << temperature << endl;
        cout << "Weights: (" << weights.page_click << ", " << weights.info_found << ", " << weights.topic_similarity << ")" << endl;

        Weights next_weights = get_next_weights(weights);
        double prob = move_probability(params, weights, next_weights, temperature);

        if (prob > uniform_real(0, 1)) {
            weights = next_weights;
        }
        temperature /= TEMP_DECREASE_RATE;
    }

    return weights;
}

int main() {
    SimParams params = read_params("params/ants.txt");
    optimize_weights(params);

    /*for (double i = 0; i < 1; i += 0.05) {
        for (double j = 0; j < 1-i; j += 0.05) {
            double k = 1 - i - j;
            Weights weights{i, j, k};
            weights = normalize(weights);
            double score = score_weights(params, weights);
            cout << score << ",";
        }
        cout << endl;
    }*/

    return 0;
}
