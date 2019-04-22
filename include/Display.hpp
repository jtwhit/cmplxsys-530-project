#pragma once

#include <string>

class Display {
public:
    virtual ~Display() = default;

    virtual void initialize_name(const std::string&) {}
    virtual void update_progress(const std::string&, int) {}

    virtual void render() = 0;
    virtual void handle_input() {}
};
