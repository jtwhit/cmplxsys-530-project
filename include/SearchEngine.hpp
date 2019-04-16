#pragma once

#include "WebPage.hpp"
#include <vector>

struct Weights {
    double page_click, info_found, topic_similarity;
};

struct ActionData {
    int page_id;
    double query, info_found, topic_similarity;
};

class SearchEngine {
public:
    SearchEngine(int num_pages, int max_info_int_, int page_length_, double page_std_dev, Weights weights_);

    void rank_pages(double query);
    void record_action(ActionData data);

    const std::vector<WebPage>& get_pages() const;

private:
    int max_info_int, page_length;
    Weights weights;
    std::vector<WebPage> web_pages;
    std::vector<ActionData> recorded_data;
    std::vector<WebPage>::iterator sorted_end;
};
