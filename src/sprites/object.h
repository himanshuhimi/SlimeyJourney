#pragma once

#include "core/config.h"

class Object
{
public:
    SDL_Renderer *renderer = nullptr;
    SDL_FRect rect, dst;
    Vector2D Position;
    Image image;
    Object(SDL_Renderer *renderer, float x, float y);
    void render(Vector2D Camera);
};