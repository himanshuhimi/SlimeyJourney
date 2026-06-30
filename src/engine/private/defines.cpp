#include "../defines.h"

int _Random_::randint(int begin, int end)
{
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(begin, end);
    return dist(rng);
};

map<int, double> durations = {
    {0, 2.5},
    {1, 0.5},
    {2, 0.5},
    {3, 0.5},
    {4, 0.5},
    {5, 1.0}};

_Random_ Random;
_Colors_ colors;
_Consts_ constants;