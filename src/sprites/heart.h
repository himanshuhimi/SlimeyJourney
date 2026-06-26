#pragma once

#include "tools/sprite.h"

class Heart : public Sprite
{
public:
    vector<Image> images = {};
    bool broken = false;
    Heart(SDL_Renderer *renderer, float x, float y);
    void handle(double dt, vector<Object> &objects);
    void render(Vector2D Camera);
};