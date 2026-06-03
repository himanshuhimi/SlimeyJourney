#pragma once

#include "../tools/sprite.h"

class Ball : public Sprite
{
public:
    Vector2D prevPos;
    bool used = false, exploded = false, mouseFollowed = false;
    Ball(SDL_Renderer *renderer, float x, float y, Vector2D Direction);
    void handle(double dt, const vector<Grass> &grasses);
    void render(Vector2D Camera);
};