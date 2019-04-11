#include "SearchEngine.hpp"
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

SearchEngine::SearchEngine(int num_pages, int max_info_int_, int page_length_, double page_std_dev, Weights weights_) :
    max_info_int(max_info_int_),
    page_length(page_length_),
    weights(weights_) {
    for (int i = 0; i < num_pages; i++) {
        web_pages.emplace_back(i, max_info_int, page_length, page_std_dev);
    }
    sorted_end = web_pages.begin();
}

void SearchEngine::rank_pages(double query) {
    unordered_map<int, double> scores;

    for (auto data : recorded_data) {
        double score = weights.page_click +
                       weights.topic_similarity * (1 - (data.topic_similarity * 2) / max_info_int) +
                       weights.info_found * (data.info_found / page_length);
        double distance = abs(query - data.query);
        scores[data.page_id] += score / (distance + 1);
    }

    sort(web_pages.begin(), sorted_end,
        [&scores](const WebPage &page1, const WebPage &page2) {
            return scores[page1.get_id()] > scores[page2.get_id()];
        });
}

void SearchEngine::record_action(ActionData data) {
    auto iter = find_if(web_pages.begin(), web_pages.end(),
        [data](const WebPage &page) {
            return page.get_id() == data.page_id;
        });

    if (iter >= sorted_end) {
        iter_swap(iter, sorted_end);
        sorted_end++;
    }

    recorded_data.push_back(data);
}

const vector<WebPage>& SearchEngine::get_pages() const {
    return web_pages;
}
