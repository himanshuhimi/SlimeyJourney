#include "../interval.h"

Interval::Interval(std::function<void()> func, double duration)
    : func(func), duration(duration)
{

}

void Interval::handle(double dt)
{
    if (!called)
        timeElapsed += dt;
    if (timeElapsed >= duration)
    {
        func();
        timeElapsed = 0;
        called = true;
    }
    else
        called = false;
}