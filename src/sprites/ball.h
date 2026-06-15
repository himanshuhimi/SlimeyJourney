#pragma once

#include "tools/sprite.h"

class Ball : public Sprite
{
public:
    Vector2D prevPos;
    map<string, Animation> anims;
    float speed = 0.0f;
    bool used = false, exploded = false, mouseFollowed = false;
    Ball(SDL_Renderer *renderer, float x, float y, string type, Vector2D Direction);
    void handle(double dt, const vector<Object> &grasses);
    void render(Vector2D Camera);
    void destroy();
};