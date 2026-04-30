#pragma once

#include "../core/config.h"

class Sprite
{
public:
    SDL_Renderer *renderer = nullptr;
    Image *image = nullptr;
    Vector2D Velocity, Position;
    SDL_FRect rect;
    Sprite(SDL_Renderer *renderer, string imgSource, float x, float y);
    void handle(double dt);
    void render();
};