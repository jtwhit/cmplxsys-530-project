#include "simulate.hpp"
#include "SearchEngine.hpp"
#include "User.hpp"
#include <iostream>

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

        const vector<WebPage>& pages = search_engine.get_pages();
        int page_index = user.choose_page(query, pages);
        const WebPage &page = pages.at(page_index);
        ActionData data = user.read_page(query, page);
        search_engine.record_action(data);

        max_depth = max(max_depth, page_index);
        num_read++;
    }

    return {max_depth, num_read};
}

vector<SimResult> simulate(SimParams params, bool print) {
    if (print)
        cout << "Creating " << params.num_pages << " web pages." << endl;
    SearchEngine search_engine(params.num_pages, params.max_info_int, params.page_length, params.page_std_dev, params.weights);

    vector<SimResult> results;
    for (int i = 0; i < params.num_users; i++) {
        if (print)
            cout << "\rUser " << (i + 1) << " / " << params.num_users << flush;

        SimResult result = iterate(params, search_engine);
        results.push_back(result);
    }
    if (print)
        cout << endl << "Done." << endl;

    return results;
}
