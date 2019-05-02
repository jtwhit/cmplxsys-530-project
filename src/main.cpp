#include "RunQueue.hpp"
#include "SingleRun.hpp"
#include "SweepRun.hpp"
#include <iostream>
#include <set>
#include <thread>
#include <experimental/filesystem>

using namespace std;
using namespace std::experimental::filesystem;

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

    create_directory("outputs");

    RunQueue run_queue;
    for (path p : paths) {
        if (p.extension() == ".param") {
            run_queue.push(make_shared<SingleRun>(p.stem(), p));
        } else if (p.extension() == ".sweep") {
            run_queue.push(make_shared<SweepRun>(p.stem(), p));
        }
    }

    run_queue.execute();

    return 0;
}
