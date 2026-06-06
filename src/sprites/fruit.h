#pragma once

#include "tools/sprite.h"

class Fruit : public Sprite
{
public:
    bool picked = false;
    string choice = "";
    Fruit(SDL_Renderer *renderer, float x, float y);
    void render(Vector2D Camera);
};