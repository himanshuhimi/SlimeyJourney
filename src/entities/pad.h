#pragma once

#include "tools/sprite.h"

class Pad : public Sprite
{
public:
    Pad(SDL_Renderer *renderer, float x, float y);
};