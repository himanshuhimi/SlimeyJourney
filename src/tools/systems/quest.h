#pragma once

#include "core/config.h"

struct Quest
{

    SDL_Renderer *renderer = nullptr;
    SDL_FRect bgRect;
    Text *text = nullptr;
    Image completeImage, bg;
    bool completed = false;
    string label = "";
    Quest(SDL_Renderer *renderer, float x, float y, string label);
    void handle(double dt);
    void render(Vector2D Camera = {});
};