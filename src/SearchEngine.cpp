#include "SearchEngine.hpp"
#include "simulate.hpp"
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <utility>

using namespace std;

SearchEngine::SearchEngine(SimProgress &progress, Weights weights_, int num_pages, int max_info_int_, int page_length_, double page_std_dev) :
    max_info_int(max_info_int_),
    page_length(page_length_),
    weights(weights_) {
    for (int i = 0; i < num_pages; i++) {
        web_pages.emplace_back(i, max_info_int, page_length, page_std_dev);
        progress.increment();
    }

    // Initially, no web pages have recorded data.
    sorted_end = web_pages.begin();
}

void SearchEngine::rank_pages(double query) {
    // Calculate scores for each web page with recorded data.
    unordered_map<int, double> scores;
    for (auto data : recorded_data) {
        // Generate score from data and weights.
        double score = weights.page_click +
                       weights.topic_similarity * (1 - (data.topic_similarity * 2) / max_info_int) +
                       weights.info_found * (data.info_found / page_length);
        // Calculate distance between the current query and the data's query.
        double distance = abs(query - data.query);
        // Update page score proportional to data score and inversely proportional to data distance.
        scores[data.page_id] += score / (distance + 1);
    }

    // Sort the pages with recorded data according to their scores.
    sort(web_pages.begin(), sorted_end,
        [&scores](const WebPage &page1, const WebPage &page2) {
            return scores[page1.get_id()] > scores[page2.get_id()];
        });
}

void SearchEngine::record_action(int page_index, ActionData data) {
    // If the page previously had no recorded data, swap it into the sorted part of
    // the vector and increment the size of the sorted part.
    auto page_iter = web_pages.begin() + page_index;
    if (page_iter >= sorted_end) {
        iter_swap(page_iter, sorted_end);
        sorted_end++;
    }

    recorded_data.push_back(data);
}

const vector<WebPage>& SearchEngine::get_pages() const {
    return web_pages;
}
