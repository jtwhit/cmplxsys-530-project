#pragma once

#include <vector>

class WebPage {
public:
    WebPage(int page_id, int max_info_int, int title_length, int page_length);

    const std::vector<int>& get_data() const;
    bool operator<(const WebPage &other) const;

private:
    int id, topic;
    std::vector<int> data, title;
};
