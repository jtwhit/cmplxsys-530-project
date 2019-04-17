#include "WebPage.hpp"
#include "distribution.hpp"
#include <tuple>

using namespace std;

WebPage::WebPage(int id_, int max_info_int, int page_length, double page_std_dev) :
    id(id_) {
    tie(topic, data) = generate_info(0, max_info_int, page_length, page_std_dev);
}

int WebPage::get_id() const {
    return id;
}

int WebPage::get_topic() const {
    return topic;
}

const set<int>& WebPage::get_data() const {
    return data;
}
