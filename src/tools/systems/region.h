#pragma once

#include "core/config.h"

class Region
{
public:
    SDL_Renderer *renderer = nullptr;
    Text display;
    string name = "";
    Region(SDL_Renderer *renderer, string name);
    void handle(double dt);
    void render(Vector2D Camera = {});
};