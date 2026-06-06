#pragma once

#include "core/config.h"

class LOS
{
public:
    SDL_Renderer *renderer = nullptr;
    SDL_FRect rect, dst;
    Vector2D Position;
    LOS(SDL_Renderer *renderer, float x, float y, float width, float height);
    void matchPosition(SDL_FRect matchRect);
    void render(Vector2D Camera);
};