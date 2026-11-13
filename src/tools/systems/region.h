#pragma once

#include "core/config.h"

struct Region
{

    SDL_Renderer *renderer = nullptr;
    string name = "";
    Region(SDL_Renderer *renderer, string name);
    void handle(double dt);
    void render(Vector2D Camera = {});
};