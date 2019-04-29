#pragma once

#include "simulate.hpp"
#include <set>

class WebPage {
public:
    WebPage(const SimParams &params, int id_);

    int get_id() const;
    int get_topic() const;
    int get_length() const;
    const std::set<int>& get_data() const;

private:
    int id, topic;
    std::set<int> data;
};
