#include "simulate.hpp"
#include "SearchEngine.hpp"
#include "User.hpp"

using namespace std;

// Run a single iteration of simulation.
SimResult iterate(SimParams params, SearchEngine &search_engine) {
    // Generate new user.
    User user(params);

    int max_depth = 0, num_read = 0;
    double old_query = -1.0;
    // Read pages until satisfied or until all pages have been read.
    while (!user.is_satisfied() && num_read < params.get_int(Param::num_pages)) {
        double query = user.generate_query();
        if (query != old_query) {
            // Only rerank if query changed.
            search_engine.rank_pages(query);
            old_query = query;
        }

        int page_index = user.choose_page(query, search_engine);
        const WebPage &page = search_engine.get_page(page_index);
        ActionData data = user.read_page(query, page);
        search_engine.record_action(page_index, data);

        // Record simulation result data.
        max_depth = max(max_depth, page_index);
        num_read++;
    }

    return {max_depth, num_read};
}

// Run simulation with given parameters.
vector<SimResult> simulate(SimParams params) {
    // Generate new search engine and pages.
    SearchEngine search_engine(params);

    int num_users = params.get_int(Param::num_users);
    vector<SimResult> results;
    for (int i = 0; i < num_users; i++) {
        SimResult result = iterate(params, search_engine);
        results.push_back(result);
    }

    return results;
}
