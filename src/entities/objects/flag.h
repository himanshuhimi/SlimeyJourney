#pragma once

#include "tools/sprite.h"

class Flag : public Sprite
{
public:
    Animation anim;
    Flag(SDL_Renderer *renderer, float x, float y);
    void handle(double dt);
    void render(Vector2D Camera);
};