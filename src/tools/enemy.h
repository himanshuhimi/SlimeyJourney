#pragma once

#include "sprite.h"

class Enemy : public Sprite
{
public:
    int speed = 0;
    SDL_FRect LOSRect, LOSdst;
    string foldertype = "";
    Enemy(SDL_Renderer *renderer, string foldertype, float x, float y, int speed);
    void handle(double dt, const vector<Grass> &grasses);
    void render(Vector2D Camera);
};