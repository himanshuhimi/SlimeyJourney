#pragma once

#include "tools/sprite.h"

struct Plate : Sprite
{
    int objID = 0;
    Plate(SDL_Renderer *renderer, float x, float y, int objID);
};