#pragma once

#include "tools/enemy.h"

class Slime : public Enemy
{
public:
    Slime(SDL_Renderer *renderer, float x, float y);
};