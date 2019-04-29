#pragma once

#include "simulate.hpp"
#include "SearchEngine.hpp"
#include "WebPage.hpp"
#include <set>
#include <string>
#include <vector>

class User {
public:
    User(const SimParams &params_);

    ActionData read_page(double query, const WebPage &page);
    int choose_page(double query, SearchEngine &search_engine) const;
    double generate_query() const;
    bool is_satisfied() const;

private:
    SimParams params;
    double satisfied_pct;
    int topic, info_length, max_depth = 0;
    std::set<int> search_info, read_pages;
};
