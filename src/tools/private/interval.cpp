#include "../interval.h"

Interval::Interval(std::function<void()> func, double duration)
    : func(func), duration(duration)
{

}

void Interval::handle(double dt)
{
    if (!called)
        timeElapsed += dt;
    std::cout << dt << std::endl;
    if (timeElapsed >= duration)
    {
        func();
        timeElapsed = 0;
        called = true;
    }
    else
        called = false;
}