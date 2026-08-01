#include "../defines.h"

int _Random_::randint(int begin, int end)
{
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(begin, end);
    return dist(rng);
};

map<string, map<int, double>> durations = {
    {"plains",
     {
         {0, 1.0},
         {1, 1.5},
         {2, 2.0},
         {3, 2.5},
         {4, 3.5},
         {5, 4.0},
         {6, 4.5},
         {7, 5.0},
         {8, 5.5},
         {9, 6.0},
         {10, 6.5},
         {11, 7.0},
     }},
    {"tutorial",
     {
         {0, 1.0},
         {1, 1.0},
         {2, 1.0},
         {3, 1.0},
         {4, 1.0},
     }}};

_Random_ Random;
_Colors_ colors;
_Consts_ constants;