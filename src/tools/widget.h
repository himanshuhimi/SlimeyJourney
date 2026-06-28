#pragma once

#include "core/config.h"

class Widget
{
public:
    SDL_Renderer *renderer = nullptr;
    SDL_FRect rect, dst;
    std::function<void()> onCallback;
    string type = "";
    Widget(SDL_Renderer *renderer, float x, float y, 
        std::function<void()> callback, string type);
};