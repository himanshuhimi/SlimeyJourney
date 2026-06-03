#pragma once

#include "../tools/sprite.h"

class Fruit : public Sprite
{
public:
    bool picked = false;
    Fruit(SDL_Renderer *renderer, float x, float y);
    void render(Vector2D Camera);
};