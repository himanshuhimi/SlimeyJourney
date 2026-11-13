#pragma once

#include "../core/config.h"

struct Interval
{
    std::function<void()> func;
    double timeElapsed = 0, duration = 0;
    bool called = false;
    Interval(std::function<void()> function, double duration = 1.0);
    void handle(double dt);
};