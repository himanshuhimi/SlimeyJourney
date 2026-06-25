#pragma once

#include "tools/sprite.h"

class Fren : public Sprite
{
public:
    bool helped = false;
    Fren(SDL_Renderer *renderer, float x, float y);
};