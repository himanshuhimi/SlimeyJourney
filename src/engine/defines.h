#pragma once

#include <iostream>
#include <random>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include "graphics.h"

using std::vector, std::map, std::unique_ptr, std::make_unique, std::unordered_map;

extern struct _Random_
{
    int randint(int begin, int end);
    template <typename T>
    T choice(vector<T> vec)
    {
        if (vec.empty())
            std::cout << "Empty Vector.";
        return vec.at(randint(0, vec.size() - 1));
    }
} Random;

extern struct _Colors_
{
    SDL_Color white = SDL_Color{255, 255, 255, 255};
    SDL_Color black = SDL_Color{0, 0, 0, 255};
    SDL_Color red = SDL_Color{179, 0, 0, 255};
    SDL_Color green = SDL_Color{0, 255, 0, 255};
    SDL_Color blue = SDL_Color{0, 0, 255, 255};
    SDL_Color skyblue = SDL_Color{97, 185, 222, 255};
    SDL_Color orange = SDL_Color{255, 165, 0, 255};
    SDL_Color yellow = SDL_Color{255, 206, 27, 255};
    SDL_Color pink = SDL_Color{255, 182, 193, 255};
    SDL_Color violet = SDL_Color{27, 76, 255, 255};
    _Colors_() {};
    SDL_Color hexToRgb(string color)
    {
        color.erase(0, color.find_first_not_of('#'));
        unsigned char r = std::stoi(color.substr(0, 2), nullptr, 16);
        unsigned char g = std::stoi(color.substr(2, 2), nullptr, 16);
        unsigned char b = std::stoi(color.substr(4, 2), nullptr, 16);
        return SDL_Color{r, g, b, 255};
    }
} colors;

extern struct _Consts_
{
    double gravity = 245;
} constants;

enum States
{
    HOME,
    LOADING,
    SETTINGS,
    PLAYING,
    PAUSED,
    PROGRESSING,
    OVER
};

extern map<int, string> titles;
extern map<int, SDL_Color> titleColors;
extern map<int, double> durations;