#pragma once

#include "tools/sprite.h"

class Fruit : public Sprite
{
public:
    bool picked = false;
    string choice = "";
    float time = 0.0f;
    int speed = 1;
    Fruit(SDL_Renderer *renderer, float x, float y);
    void handle(double dt, const vector<Object> &objects);
    void render(Vector2D Camera);
};