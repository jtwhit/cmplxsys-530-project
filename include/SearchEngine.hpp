#pragma once

#include "WebPage.hpp"
#include <vector>

// Weights given to different types of data. Absolute magnitude of weights does
// not matter, only their magnitudes relative to each other.
struct Weights {
    double page_click, info_found, topic_similarity;
};

// Data collected by search engine about user actions.
struct ActionData {
    int page_id;
    double query, info_found, topic_similarity;
};

// Creates and stores web pages, ranks them according to queries, and stores user action data.
class SearchEngine {
public:
    // Initialize search engine with given weights, creating num_pages pages with given parameters.
    SearchEngine(Weights weights_, int num_pages, int max_info_int_, int page_length_, double page_std_dev);

    void rank_pages(double query);
    void record_action(int page_index, ActionData data);

    const std::vector<WebPage>& get_pages() const;

private:
    int max_info_int, page_length;
    Weights weights;
    std::vector<WebPage> web_pages;
    std::vector<ActionData> recorded_data;
    std::vector<WebPage>::iterator sorted_end;
};
