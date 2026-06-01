#pragma once

#include "../tools/sprite.h"

class Ball : public Sprite
{
public:
    Ball(SDL_Renderer *renderer, float x, float y);
    void handle(double dt, const vector<Grass> &grasses);
};