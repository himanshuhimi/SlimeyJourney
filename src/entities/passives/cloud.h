#pragma once

#include "tools/sprite.h"

struct Cloud : public Sprite
{

    bool renderable = true;
    Cloud(SDL_Renderer *renderer);
    void handle(double dt);
    void render(Vector2D Camera = {});
};