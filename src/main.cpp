#include "simulate.hpp"
#include "PathQueue.hpp"
#include <algorithm>
#include <atomic>
#include <fstream>
#include <functional>
#include <iostream>
#include <set>
#include <thread>
#include <vector>
#include <experimental/filesystem>

#ifdef HAS_NCURSES
#include "NCursesDisplay.hpp"
#else
#include "ConsoleDisplay.hpp"
#endif

using namespace std;
using namespace std::experimental::filesystem;

void run_sim(path sim_path, shared_ptr<Display> display) {
    SimParams params = read_params(sim_path);
    vector<SimResult> run_results = simulate(params, display);

    path output_path("outputs");
    output_path /= sim_path.filename();
    ofstream output(output_path);
    for (SimResult result : run_results) {
        output << result.list_depth << ", " << result.pages_read << "\n";
    }
}

void run_sim_worker(PathQueue &path_queue, atomic_bool &done, shared_ptr<Display> display) {
    while (!path_queue.empty()) {
        path sim_path = path_queue.pop();
        run_sim(sim_path, display);
    }
    done = true;
}

void run_sims(const set<path> &paths) {
    create_directory("outputs");

    #ifdef HAS_NCURSES
    shared_ptr<Display> display = make_shared<NCursesDisplay>();
    #else
    shared_ptr<Display> display = make_shared<ConsoleDisplay>();
    #endif

    PathQueue path_queue;
    for (path p : paths) {
        display->initialize_name(p.string());
        path_queue.push(p);
    }

    unsigned int num_threads = thread::hardware_concurrency();
    vector<thread> threads;
    vector<atomic_bool> done_flags(num_threads);
    for (unsigned int i = 0; i < num_threads; i++) {
        threads.emplace_back(run_sim_worker, ref(path_queue), ref(done_flags[i]), display);
    }

    while (count(done_flags.begin(), done_flags.end(), false) > 0) {
        display->render();
        display->handle_input();
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Error: Must specify at least one parameter file or directory." << endl;
        cerr << "Usage: " << argv[0] << " PARAM_PATH [PARAM_PATH ...]" << endl;
        return 1;
    }

    set<path> paths;
    for (int i = 1; i < argc; i++) {
        path new_path(argv[i]);
        if (!exists(new_path)) {
            cerr << "Error: " << new_path << " does not exist." << endl;
            return 1;
        }

        if (is_directory(new_path)) {
            for (auto sub_path : directory_iterator(new_path)) {
                if (is_regular_file(sub_path)) {
                    paths.insert(sub_path);
                }
            }
        } else if (is_regular_file(new_path)) {
            paths.insert(new_path);
        }
    }

    run_sims(paths);

    return 0;
}
