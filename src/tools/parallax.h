#pragma once

#include "core/config.h"

class Parallax
{
public:
    SDL_Renderer *renderer = nullptr;
    map<int, Image> images = {};
    SDL_FRect rect = {};
    string directory = "data/assets/ui/parallax/";
    int degree = 0;
    Parallax(SDL_Renderer *renderer);
    void render(Vector2D Camera = {});
};