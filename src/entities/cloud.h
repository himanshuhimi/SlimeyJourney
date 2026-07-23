#pragma once

#include "tools/sprite.h"

class Cloud : public Sprite
{
public:
    bool renderable = true;
    Cloud(SDL_Renderer *renderer);
    void handle(double dt);
    void render(Vector2D Camera = {});
};