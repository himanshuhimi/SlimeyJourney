#pragma once

#include "core/config.h"

class Quest
{
public:
    SDL_Renderer *renderer = nullptr;
    Text *text = nullptr;
    Image completeImage;
    bool completed = false;
    string label = "";
    Quest(SDL_Renderer *renderer, float x, float y, string label);
    void handle(double dt);
    void render(Vector2D Camera = {0.0f, 0.0f});
};