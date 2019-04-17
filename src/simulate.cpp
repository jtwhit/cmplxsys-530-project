#include "simulate.hpp"
#include "SearchEngine.hpp"
#include "User.hpp"
#include <iomanip>
#include <iostream>

using namespace std;

SimProgress::SimProgress(const string &name_) :
    name(name_) {}

void SimProgress::set_target(int target) {
    lock_guard progress_guard(progress_mutex);
    total_ops = target;
}

void SimProgress::increment() {
    lock_guard progress_guard(progress_mutex);
    done_ops++;
}

bool SimProgress::working() {
    lock_guard progress_guard(progress_mutex);
    return (total_ops == 0) || (done_ops < total_ops);
}

ostream& operator<<(ostream &output, SimProgress &progress) {
    lock_guard progress_guard(progress.progress_mutex);
    int done_pct = 0;
    if (progress.total_ops > 0) {
        double done_ratio = static_cast<double>(progress.done_ops) / progress.total_ops;
        done_pct = static_cast<int>(done_ratio * 100);
    }
    output << left << setw(40) << progress.name << " " << right << setw(3) << done_pct << "%";

    return output;
}

SimResult iterate(SimParams params, SearchEngine &search_engine) {
    User user(params.max_info_int, params.user_length, params.user_std_dev, params.user_sat_pct);

    int max_depth = 0, num_read = 0;
    double old_query = -1.0;
    while (!user.is_satisfied() && num_read < params.num_pages) {
        double query = user.generate_query();
        if (query != old_query) {
            search_engine.rank_pages(query);
            old_query = query;
        }

        const vector<WebPage>& pages = search_engine.get_pages();
        int page_index = user.choose_page(query, pages);
        const WebPage &page = pages.at(page_index);
        ActionData data = user.read_page(query, page);
        search_engine.record_action(page_index, data);

        max_depth = max(max_depth, page_index);
        num_read++;
    }

    return {max_depth, num_read};
}

vector<SimResult> simulate(SimParams params, SimProgress &progress) {
    progress.set_target(params.num_pages + params.num_users);

    SearchEngine search_engine(progress, params.weights, params.num_pages, params.max_info_int, params.page_length, params.page_std_dev);

    vector<SimResult> results;
    for (int i = 0; i < params.num_users; i++) {
        SimResult result = iterate(params, search_engine);
        results.push_back(result);
        progress.increment();
    }

    return results;
}
