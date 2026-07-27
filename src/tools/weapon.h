#pragma once

#include "sprite.h"

class Weapon : public Sprite
{
public:
    Vector2D prevPos{}, Direction{};
    float speed = 0.0f;
    bool used = false;
    Weapon(SDL_Renderer *renderer, string imgSource, 
        float x, float y, Vector2D Direction);
    void handle(double dt, const vector<Object> &objects);
    void render(Vector2D Camera);
    void destroy();
};