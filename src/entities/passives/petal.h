#pragma once

#include "tools/sprite.h"

struct Petal : public Sprite
{

    string choice = "";
    Petal(SDL_Renderer *renderer);
    void handle(double dt);
};