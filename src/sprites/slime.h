#pragma once

#include "enemy.h"

class Slime : public Enemy
{
public:
    Slime(SDL_Renderer *renderer, float x, float y);
};