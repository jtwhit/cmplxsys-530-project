#include "simulate.hpp"
#include "distribution.hpp"
#include <iostream>
#include <cmath>
#include <future>

using namespace std;

// Simulated annealing start point.
const double INITIAL_TEMPERATURE = 0.1;
// Simulated annealing rate.
const double TEMP_DECREASE_RATE = 1.1;
// Perturbation of weights on each iteration.
const double PERTURB_AMOUNT = 0.1;
// Number of times to run simulation and average results.
const int NUM_RUNS = 100;
// Number of simulated annealing iterations.
const int NUM_ITERATIONS = 100;

// Search engine weights.
struct Weights {
    double page_click;
    double info_found;
    double topic_similarity;
};

// Score the given weights by running simulation using them.
double score_weights(SimParams params, Weights weights) {
    params.set(Param::page_click_weight, weights.page_click);
    params.set(Param::info_found_weight, weights.info_found);
    params.set(Param::topic_similarity_weight, weights.topic_similarity);

    // Find average pages read over many simulations.
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

// Normalize given weights to be > 0 and sum to 1.
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

// Generate weight perturbation.
double create_delta() {
    int delta = (uniform_int(0, 1) * 2) - 1;
    return delta * PERTURB_AMOUNT;
}

// Create new weight by perturbing current weight.
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

// Get probability of moving into next state.
// Very likely if temperature is high.
// Always happens if next state is better.
// Tends to 0 if next state is worse as temperature decreases.
double move_probability(SimParams params, Weights current_weights, Weights next_weights, double temperature) {
    // Score current and next state weights.
    // Run in parallel for efficiency.
    auto current_handle = async(launch::async, score_weights, params, current_weights);
    auto next_handle = async(launch::async, score_weights, params, next_weights);

    double current_score = current_handle.get();
    double next_score = next_handle.get();
    cout << current_score << endl;
    double ratio = current_score / next_score;

    return pow(ratio, 1.0 / temperature);
}

// Find optimal weights by running simulated annealing.
Weights optimize_weights(const SimParams &params) {
    Weights weights;
    weights.page_click = params.get_real(Param::page_click_weight);
    weights.info_found = params.get_real(Param::info_found_weight);
    weights.topic_similarity = params.get_real(Param::topic_similarity_weight);

    double temperature = INITIAL_TEMPERATURE;
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        cout << temperature << "," << weights.page_click << "," << weights.info_found << "," << weights.topic_similarity << ",";

        // Find next state.
        Weights next_weights = get_next_weights(weights);

        // Move to next state if probability is high enough.
        double prob = move_probability(params, weights, next_weights, temperature);
        if (prob > uniform_real(0, 1)) {
            weights = next_weights;
        }

        // Decrease randomness.
        temperature /= TEMP_DECREASE_RATE;
    }

    return weights;
}

int main() {
    SimParams params("params/ants.param");
    optimize_weights(params);

    return 0;
}
