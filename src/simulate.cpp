#include "simulate.hpp"
#include "SearchEngine.hpp"
#include "User.hpp"

using namespace std;

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

        int page_index = user.choose_page(query, search_engine);
        const WebPage &page = search_engine.get_page(page_index);
        ActionData data = user.read_page(query, page);
        search_engine.record_action(page_index, data);

        max_depth = max(max_depth, page_index);
        num_read++;
    }

    return {max_depth, num_read};
}

vector<SimResult> simulate(SimParams params, Display &display) {
    SearchEngine search_engine(params.weights, params.num_pages, params.max_info_int, params.page_length, params.page_std_dev);

    vector<SimResult> results;
    for (int i = 0; i < params.num_users; i++) {
        SimResult result = iterate(params, search_engine);
        results.push_back(result);

        double progress_ratio = static_cast<double>(i) / params.num_users;
        int progress_pct = static_cast<int>(progress_ratio * 100);
        display.update_progress(params.name, progress_pct);
    }
    display.update_progress(params.name, 100);

    return results;
}
