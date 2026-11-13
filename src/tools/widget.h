#pragma once

#include "core/config.h"

using UIFunction = std::function<void()>;

struct Widget
{

    SDL_Renderer *renderer = nullptr;
    SDL_FRect rect, dst;
    UIFunction onCallback;
    Audio clickAudio;
    Widget(SDL_Renderer *renderer, float x, float y,
           UIFunction callback = []{});
    virtual void render(Vector2D Camera = {}) {};
    virtual void handle(double dt) {};
    virtual void update(SDL_Event event) {};
};