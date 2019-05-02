#include "SearchEngine.hpp"
#include "simulate.hpp"
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <utility>

using namespace std;

SearchEngine::SearchEngine(const SimParams &params_) :
    params(params_) {}

void SearchEngine::rank_pages(double query) {
    // Calculate scores for each web page with recorded data.
    unordered_map<int, double> scores;
    for (auto data : recorded_data) {
        // Generate score from data and weights.
        double score = params.get_real(Param::page_click_weight) +
                       params.get_real(Param::topic_similarity_weight) * data.topic_similarity +
                       params.get_real(Param::info_found_weight) * data.info_found;
        // Calculate distance between the current query and the data's query.
        double distance = abs(query - data.query);
        // Update page score proportional to data score and inversely proportional to data distance.
        scores[data.page_id] += score / (distance + 1);
    }

    // Sort the pages with recorded data according to their scores.
    sort(web_pages.begin(), web_pages.begin() + num_sorted,
        [&scores](const WebPage &page1, const WebPage &page2) {
            return scores[page1.get_id()] > scores[page2.get_id()];
        });
}

void SearchEngine::record_action(int page_index, ActionData data) {
    // If the page previously had no recorded data, swap it into the sorted part of
    // the vector and increment the size of the sorted part.
    if (page_index >= num_sorted) {
        iter_swap(web_pages.begin() + page_index, web_pages.begin() + num_sorted);
        num_sorted++;
    }

    recorded_data.push_back(data);
}

const WebPage& SearchEngine::get_page(int index) {
    // Lazily generate web pages.
    // Saves a ton of time and memory with no effect on simulation.
    while (index >= static_cast<int>(web_pages.size())) {
        web_pages.emplace_back(params, web_pages.size());
    }

    return web_pages[index];
}

int SearchEngine::get_num_pages() const {
    return params.get_int(Param::num_pages);
}
