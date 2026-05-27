#pragma once

#include "../tools/sprite.h"

class Fruit : public Sprite
{
public:
    double percentage = 0.0;
    bool taken = false;
    Fruit(SDL_Renderer *renderer, float x, float y);
    void render(Vector2D Camera);
};