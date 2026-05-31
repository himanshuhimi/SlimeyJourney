#pragma once

#include "../core/config.h"

class Grass
{
public:
    SDL_Renderer *renderer = nullptr;
    Image image;
    SDL_FRect rect, dst;
    Vector2D Position;
    Grass(SDL_Renderer *renderer, float x, float y);
    void render(Vector2D Camera);
};