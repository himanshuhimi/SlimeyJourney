#include "../defines.h"

int _Random_::randint(int begin, int end)
{
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(begin, end);
    return dist(rng);
};

map<int, double> durations = {
    {0, 0.2},
    {1, 0.5},
    {2, 0.7},
    {3, 1.0},
    {4, 1.2},
    {5, 1.5},
    {6, 1.7},
    {7, 2.0},
    {8, 2.2},
    {9, 2.5},
    {10, 2.7},
    {11, 3.0},
    {12, 3.2},
    {13, 3.5},
    {14, 3.7},
    {15, 4.0},
    {16, 4.2},
    {17, 4.5},
};

_Random_ Random;
_Colors_ colors;
_Consts_ constants;