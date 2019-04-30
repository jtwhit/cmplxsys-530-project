#include "simulate.hpp"
#include "SearchEngine.hpp"
#include "User.hpp"
#include "Display.hpp"
#include <iostream>

using namespace std;

SimResult iterate(SimParams params, SearchEngine &search_engine) {
    //cout << "gen user" << endl;
    User user(params);
    //cout << "done gen" << endl;

    int max_depth = 0, num_read = 0;
    double old_query = -1.0;
    while (!user.is_satisfied() && num_read < params.num_pages) {
        //cout << num_read << endl;
        //cout << "query" << endl;
        double query = user.generate_query();
        if (query != old_query) {
            search_engine.rank_pages(query);
            old_query = query;
        }

        //cout << "choose" << endl;
        int page_index = user.choose_page(query, search_engine);
        //cout << "read" << endl;
        const WebPage &page = search_engine.get_page(page_index);
        ActionData data = user.read_page(query, page);
        //cout << "record" << endl;
        search_engine.record_action(page_index, data);

        max_depth = max(max_depth, page_index);
        num_read++;
    }

    return {max_depth, num_read};
}

vector<SimResult> simulate(SimParams params, shared_ptr<Display> display) {
    SearchEngine search_engine(params);

    vector<SimResult> results;
    for (int i = 0; i < params.num_users; i++) {
        //cout << "iterate" << endl;
        SimResult result = iterate(params, search_engine);
        results.push_back(result);

        double progress_ratio = static_cast<double>(i) / params.num_users;
        int progress_pct = static_cast<int>(progress_ratio * 100);

        if (display) {
            display->update_progress(params.name, progress_pct);
        }
    }
    if (display) {
        display->update_progress(params.name, 100);
    }
    //cout << "done" << endl;

    return results;
}
