#pragma once

#include "simulate.hpp"
#include "WebPage.hpp"
#include <vector>

// Data collected by search engine about user actions.
struct ActionData {
    int page_id;
    double query, info_found, topic_similarity;
};

// Creates and stores web pages, ranks them according to queries, and stores user action data.
class SearchEngine {
public:
    // Initialize search engine with given weights, creating num_pages pages with given parameters.
    SearchEngine(const SimParams &params_);

    void rank_pages(double query);
    void record_action(int page_index, ActionData data);
    const WebPage& get_page(int index);

    int get_num_pages() const;

private:
    SimParams params;
    int num_sorted = 0;
    std::vector<WebPage> web_pages;
    std::vector<ActionData> recorded_data;
};
