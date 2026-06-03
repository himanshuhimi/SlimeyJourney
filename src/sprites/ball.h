#pragma once

#include "../tools/sprite.h"

class Ball : public Sprite
{
public:
    bool used = false, exploded = false, mouseFollowed = false;
    Vector2D prevPos;
    std::unordered_map<string, Animation *> anims;
    Ball(SDL_Renderer *renderer, float x, float y, Vector2D Direction);
    void handle(double dt, const vector<Grass> &grasses);
    void render(Vector2D Camera);
};