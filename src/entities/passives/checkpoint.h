#pragma once

#include "tools/sprite.h"
#include "tools/systems/dialogue.h"

struct Checkpoint : public Sprite
{

    bool helped = false;
    Dialogue *dialogue;
    Checkpoint(SDL_Renderer *renderer, float x, float y);
    void handle(double dt, vector<Object> &objects);
    void render(Vector2D Camera);
};