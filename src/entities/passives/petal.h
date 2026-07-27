#pragma once

#include "tools/sprite.h"

class Petal : public Sprite
{
public:
    Petal(SDL_Renderer *renderer);
    void handle(double dt);
};