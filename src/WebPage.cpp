#include "WebPage.hpp"
#include "distribution.hpp"
#include <tuple>

using namespace std;

WebPage::WebPage(const SimParams &params, int id_) :
    id(id_) {
    int page_length = uniform_int(params.get_int(Param::page_length_min), params.get_int(Param::page_length_max));
    double page_std_dev = uniform_real(params.get_real(Param::page_std_dev_min), params.get_real(Param::page_std_dev_max));
    // Randomly choose topic and normally distribute info around it.
    tie(topic, data) = generate_info(0, params.get_int(Param::max_info_int), page_length, page_std_dev);
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
