#pragma once

#include "tools/sprite.h"
#include "tools/systems/dialogue.h"

class Fren : public Sprite
{
public:
    bool helped = false;
    Dialogue *dialogue;
    Fren(SDL_Renderer *renderer, float x, float y);
    void handle(double dt, vector<Object> &objects);
    void render(Vector2D Camera);
};