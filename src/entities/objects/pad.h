#pragma once

#include "tools/sprite.h"

struct Pad : public Sprite
{
    int inUse = false;
    Image usingImg, notUsingImg;
    Pad(SDL_Renderer *renderer, float x, float y);
    void render(Vector2D Camera);
};