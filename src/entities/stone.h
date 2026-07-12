#pragma once

#include "tools/sprite.h"

class Stone : public Sprite
{
public:
    int destructLvl = 0, maxDestructLvl = 5;
    bool destructed = false;
    Stone(SDL_Renderer *renderer, float x, float y);
    void render(Vector2D Camera = {});
    void handle(double dt, vector<Object> &objects);
    void destruct(int byPoints = 1);
};