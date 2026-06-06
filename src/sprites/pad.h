#pragma once

#include "tools/sprite.h"

class Pad : public Sprite
{
public:
    int incPrecent = 0;
    Pad(SDL_Renderer *renderer, float x, float y, int incPercent);
};