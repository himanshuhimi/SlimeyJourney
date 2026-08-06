#pragma once

#include "tools/sprite.h"

class Petal : public Sprite
{
public:
    string choice = "";
    Petal(SDL_Renderer *renderer);
    void handle(double dt);
};