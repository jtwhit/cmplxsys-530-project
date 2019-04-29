#include "WebPage.hpp"
#include "distribution.hpp"
#include <tuple>
#include <iostream>

using namespace std;

WebPage::WebPage(const SimParams &params, int id_) :
    id(id_) {
    int page_length = uniform_int(params.page_length_min, params.page_length_max);
    double page_std_dev = uniform_real(params.page_std_dev_min, params.page_std_dev_max);
    tie(topic, data) = generate_info(0, params.max_info_int, page_length, page_std_dev);
    //cout << data.size() << endl;
}

int WebPage::get_id() const {
    return id;
}

int WebPage::get_topic() const {
    return topic;
}

int WebPage::get_length() const {
    return data.size();
}

const set<int>& WebPage::get_data() const {
    return data;
}
