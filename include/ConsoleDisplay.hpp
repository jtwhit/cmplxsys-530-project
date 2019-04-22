#pragma once

#include "Display.hpp"
#include <set>
#include <mutex>

class ConsoleDisplay : public Display {
public:
    void update_progress(const std::string &name, int progress) override;

    void render() override;

private:
    std::set<std::string> started, running, ended;
    std::mutex display_mutex;
};
