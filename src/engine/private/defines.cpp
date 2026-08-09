#include "../defines.h"

int _Random_::randint(int begin, int end)
{
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(begin, end);
    return dist(rng);
};

_Random_ Random;
_Colors_ colors;
_Consts_ constants;